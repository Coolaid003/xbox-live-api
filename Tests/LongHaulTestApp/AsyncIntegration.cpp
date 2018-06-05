#include "pch.h"
#include "GameLogic\Game.h"

using namespace LongHaulTestApp;

class win32_handle
{
public:
    win32_handle() : m_handle(nullptr)
    {
    }

    ~win32_handle()
    {
        if (m_handle != nullptr) CloseHandle(m_handle);
        m_handle = nullptr;
    }

    void set(HANDLE handle)
    {
        m_handle = handle;
    }

    HANDLE get() { return m_handle; }

private:
    HANDLE m_handle;
};

win32_handle g_stopRequestedHandle;
win32_handle g_workReadyHandle;

void CALLBACK HandleAsyncQueueCallback(
    _In_ void* context,
    _In_ async_queue_handle_t queue,
    _In_ AsyncQueueCallbackType type
)
{
    UNREFERENCED_PARAMETER(context);
    UNREFERENCED_PARAMETER(queue);

    switch (type)
    {
    case AsyncQueueCallbackType::AsyncQueueCallbackType_Work:
        ReleaseSemaphore(g_workReadyHandle.get(), 1, nullptr);
        break;
    }
}


DWORD WINAPI game_thread_proc(LPVOID lpParam)
{
    async_queue_handle_t queue;
    uint32_t sharedAsyncQueueId = *((uint32_t*)lpParam);
    CreateSharedAsyncQueue(
        sharedAsyncQueueId,
        AsyncQueueDispatchMode::AsyncQueueDispatchMode_Manual,
        AsyncQueueDispatchMode::AsyncQueueDispatchMode_Manual,
        &queue);

    bool stop = false;
    while (!stop)
    {
        // Game Work
        try
        {
            if (g_sampleInstance->m_testsStarted)
            {
                g_sampleInstance->HandleTests();
            }
        }
        catch (const std::exception& e)
        {
            g_sampleInstance->Log("[Error] " + std::string(e.what()));
        }

        // Dispatching async completions on game thread
        while(!IsAsyncQueueEmpty(queue, AsyncQueueCallbackType_Completion))
        {
            DispatchAsyncQueue(queue, AsyncQueueCallbackType_Completion, 0);
        }
    }

    CloseAsyncQueue(queue);
    return 0;
}

DWORD WINAPI work_thread_proc(LPVOID lpParam)
{
    HANDLE hEvents[2] =
    {
        g_workReadyHandle.get(),
        g_stopRequestedHandle.get()
    };

    async_queue_handle_t queue;
    uint32_t sharedAsyncQueueId = *((uint32_t*)lpParam);
    CreateSharedAsyncQueue(
        sharedAsyncQueueId,
        AsyncQueueDispatchMode::AsyncQueueDispatchMode_Manual,
        AsyncQueueDispatchMode::AsyncQueueDispatchMode_Manual,
        &queue);

    bool stop = false;
    while (!stop)
    {
        DWORD dwResult = WaitForMultipleObjectsEx(2, hEvents, false, INFINITE, false);
        switch (dwResult)
        {
        case WAIT_OBJECT_0: // work ready
            DispatchAsyncQueue(queue, AsyncQueueCallbackType_Work, 0);
            break;

        default:
            stop = true;
            break;
        }
    }

    CloseAsyncQueue(queue);
    return 0;
}

uint32_t QUEUE_ID = 1;

void Game::InitializeAsync()
{
    g_stopRequestedHandle.set(CreateEvent(nullptr, true, false, nullptr));
    g_workReadyHandle.set(CreateSemaphore(nullptr, 0, LONG_MAX, nullptr));

    CreateSharedAsyncQueue(
        QUEUE_ID,
        AsyncQueueDispatchMode::AsyncQueueDispatchMode_Manual,
        AsyncQueueDispatchMode::AsyncQueueDispatchMode_Manual,
        &m_queue);
    AddAsyncQueueCallbackSubmitted(m_queue, nullptr, HandleAsyncQueueCallback, &m_callbackToken);
    
    m_backgroundThreads.push_back(CreateThread(nullptr, 0, game_thread_proc, &QUEUE_ID, 0, nullptr));
    m_backgroundThreads.push_back(CreateThread(nullptr, 0, work_thread_proc, &QUEUE_ID, 0, nullptr));
}