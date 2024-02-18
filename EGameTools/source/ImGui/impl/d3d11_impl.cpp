﻿#include <pch.h>
#include "..\menu\menu.h"
#include "..\menu\init.h"

namespace impl {
	namespace d3d11 {
		ID3D11Device* d3d11Device = nullptr;
		static ID3D11DeviceContext* d3d11DeviceContext = nullptr;
		static ID3D11RenderTargetView* d3d11RenderTargetView = nullptr;

		HRESULT(__stdcall* oPresent)(IDXGISwapChain*, UINT, UINT);
		HRESULT __stdcall hkPresent11(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags) {
			static bool init = false;

			if (!init) {
				DXGI_SWAP_CHAIN_DESC desc{};
				pSwapChain->GetDesc(&desc);

				pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&d3d11Device);
				d3d11Device->GetImmediateContext(&d3d11DeviceContext);

				ID3D11Texture2D* renderTargetTexture = nullptr;

				// Create custom render target view because DX11 color space issue with ImGui bruh
				if (SUCCEEDED(pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<LPVOID*>(&renderTargetTexture)))) {
					D3D11_RENDER_TARGET_VIEW_DESC desc{};
					memset(&desc, 0, sizeof(desc));
					desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
					desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

					d3d11Device->CreateRenderTargetView(renderTargetTexture, &desc, &d3d11RenderTargetView);
					renderTargetTexture->Release();
				}

#ifndef LLMH_IMPL_DISABLE_DEBUG
				std::thread([&desc]() { impl::win32::init(desc.OutputWindow); }).detach();
#else 
				impl::win32::init(desc.OutputWindow);
#endif

				ImGui::CreateContext();
				ImGui::GetIO().IniFilename = nullptr;

				ImGui_ImplWin32_Init(desc.OutputWindow);
				ImGui_ImplDX11_Init(d3d11Device, d3d11DeviceContext);

				Menu::InitImGui();

				init = true;
			}

			d3d11DeviceContext->OMSetRenderTargets(1, &d3d11RenderTargetView, nullptr);

			ImGui_ImplDX11_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();

			Menu::FirstTimeRunning();
			if (Menu::menuToggle.GetValue())
				Menu::Render();

			ImGui::EndFrame();
			ImGui::Render();
			ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

			return oPresent(pSwapChain, SyncInterval, Flags);
		}

		void init() {
			assert(kiero::bind(8, (LPVOID*)&oPresent, hkPresent11) == kiero::Status::Success);
		}
	}
}