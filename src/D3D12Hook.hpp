#pragma once

#include <iostream>
#include <functional>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi")

#include <d3d12.h>
#include <dxgi1_4.h>

#include "utility/FunctionHook.hpp"

class D3D12Hook
{
public:
	typedef std::function<void(D3D12Hook&)> OnPresentFn;
	typedef std::function<void(D3D12Hook&)> OnResizeBuffersFn;
    typedef std::function<void(D3D12Hook&)> OnResizeTargetFn;
    typedef std::function<void(D3D12Hook&)> OnCreateSwapChainFn;

	D3D12Hook() = default;
	virtual ~D3D12Hook();

	bool hook();
	bool unhook();

    bool is_hooked() {
        return m_hooked;
    }

    void on_present(OnPresentFn fn) {
        m_on_present = fn;
    }

    void on_resize_buffers(OnResizeBuffersFn fn) {
        m_on_resize_buffers = fn;
    }

    void on_resize_target(OnResizeTargetFn fn) {
        m_on_resize_target = fn;
    }

    void on_create_swap_chain(OnCreateSwapChainFn fn) {
        m_on_create_swap_chain = fn;
    }

    ID3D12Device4* get_device() const {
        return m_device;
    }

    IDXGISwapChain3* get_swap_chain() const {
        return m_swap_chain;
    }

    ID3D12CommandQueue* get_command_queue() const {
        return m_command_queue;
    }

    UINT get_display_width() const {
        return m_display_width;
    }

    UINT get_display_height() const {
        return m_display_height;
    }

    UINT get_render_width() const {
        return m_render_width;
    }

    UINT get_render_height() const {
        return m_render_height;
    }

protected:
    ID3D12Device4* m_device{ nullptr };
    IDXGISwapChain3* m_swap_chain{ nullptr };
    ID3D12CommandQueue* m_command_queue{ nullptr };
    UINT m_display_width{ NULL };
    UINT m_display_height{ NULL };
    UINT m_render_width{ NULL };
    UINT m_render_height{ NULL };

    bool m_hooked{ false };

    std::unique_ptr<FunctionHook> m_present_hook{};
    std::unique_ptr<FunctionHook> m_resize_buffers_hook{};
    std::unique_ptr<FunctionHook> m_resize_target_hook{};
    std::unique_ptr<FunctionHook> m_create_swap_chain_hook{};

    OnPresentFn m_on_present{ nullptr };
    OnResizeBuffersFn m_on_resize_buffers{ nullptr };
    OnResizeTargetFn m_on_resize_target{ nullptr };
    OnCreateSwapChainFn m_on_create_swap_chain{ nullptr };
    
    static HRESULT WINAPI present(IDXGISwapChain3* swap_chain, UINT sync_interval, UINT flags);
    static HRESULT WINAPI resize_buffers(IDXGISwapChain3* swap_chain, UINT buffer_count, UINT width, UINT height, DXGI_FORMAT new_format, UINT swap_chain_flags);
    static HRESULT WINAPI resize_target(IDXGISwapChain3* swap_chain, const DXGI_MODE_DESC* new_target_parameters);
    static HRESULT WINAPI create_swap_chain(IDXGIFactory4* factory, IUnknown* device, HWND hwnd, const DXGI_SWAP_CHAIN_DESC* desc, const DXGI_SWAP_CHAIN_FULLSCREEN_DESC* p_fullscreen_desc, IDXGIOutput* p_restrict_to_output, IDXGISwapChain** swap_chain);
};
