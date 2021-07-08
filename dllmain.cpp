// dllmain.cpp : Определяет точку входа для приложения DLL.
#include "pch.h"
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <wchar.h>

#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include <wincodec.h>
#pragma comment(lib,"windowscodecs.lib")
#pragma comment(lib,"d2d1.lib")
#pragma comment(lib,"dwrite.lib")
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

#ifndef WINVER              // Allow use of features specific to Windows XP or later.
#define WINVER 0x0501       // Change this to the appropriate value to target other versions of Windows.
#endif

#ifndef _WIN32_WINNT        // Allow use of features specific to Windows XP or later.
#define _WIN32_WINNT 0x0501 // Change this to the appropriate value to target other versions of Windows.
#endif

#ifndef _WIN32_WINDOWS      // Allow use of features specific to Windows 98 or later.
#define _WIN32_WINDOWS 0x0410 // Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE           // Allow use of features specific to IE 6.0 or later.
#define _WIN32_IE 0x0600    // Change this to the appropriate value to target other versions of IE.
#endif

#ifndef UNICODE
#define UNICODE
#endif

#define WIN32_LEAN_AND_MEAN     // Exclude rarely-used stuff from Windows headers
// Windows Header Files:


template<class Interface>
inline void
SafeRelease(
    Interface** ppInterfaceToRelease
)
{
    if (*ppInterfaceToRelease != NULL)
    {
        (*ppInterfaceToRelease)->Release();

        (*ppInterfaceToRelease) = NULL;
    }
}

#ifndef Assert
#if defined( DEBUG ) || defined( _DEBUG )
#define Assert(b) do {if (!(b)) {OutputDebugStringA("Assert: " #b "\n");}} while(0)
#else
#define Assert(b)
#endif //DEBUG || _DEBUG
#endif

#ifndef HINST_THISCOMPONENT
EXTERN_C IMAGE_DOS_HEADER __ImageBase;
#define HINST_THISCOMPONENT ((HINSTANCE)&__ImageBase)
#endif

struct MyBitmap
{
    int width;
    int height;
    unsigned char* memory;

    MyBitmap(int w, int h)
    {
        width = w;
        height = h;
        memory = new unsigned char[w * h * 4];
    }

    ~MyBitmap()
    {
        delete[] memory;
    }

    void SetPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b, unsigned char a = 0)
    {
        if (x >= 0 && x < width)
        {
            if (y >= 0 && y < height)
            {
                int ofs = (y * width + x) * 4;

                memory[ofs + 0] = b;
                memory[ofs + 1] = g;
                memory[ofs + 2] = r;
                memory[ofs + 3] = a;
            }
        }
    }
};


extern "C"  __declspec(dllexport) void SaveToImageFile(double* mass, int x, int y)
{
    HRESULT hr = S_OK;
    double max = 0;
    for (int i = 0; i <= x * y; i++) 
    {
        if (mass[i] > max) {
            max = mass[i];
        }
    }

    MyBitmap bm(x, y);
    for (int i = 0; i < x; i++)
    {
        for (int j = 0; j < y; j++)
        {
            double jex = mass[i * y + j] / max;
            bm.SetPixel(i, j, jex * 255, 0, 0);
        }
    }
    ID2D1Bitmap* bitmap;
    IWICImagingFactory* pWICFactory = NULL;
    ID2D1Factory* pD2DFactory = NULL;
    IDWriteFactory* pDWriteFactory = NULL;
    IWICBitmap* pWICBitmap = NULL;
    ID2D1RenderTarget* pRT = NULL;
    IDWriteTextFormat* pTextFormat = NULL;
    ID2D1PathGeometry* pPathGeometry = NULL;
    ID2D1GeometrySink* pSink = NULL;
    ID2D1GradientStopCollection* pGradientStops = NULL;
    ID2D1LinearGradientBrush* pLGBrush = NULL;
    ID2D1SolidColorBrush* pBlackBrush = NULL;
    IWICBitmapEncoder* pEncoder = NULL;
    IWICBitmapFrameEncode* pFrameEncode = NULL;
    IWICStream* pStream = NULL;

    hr = CoCreateInstance(
        CLSID_WICImagingFactory,
        NULL,
        CLSCTX_INPROC_SERVER,
        IID_IWICImagingFactory,
        reinterpret_cast<void**>(&pWICFactory)
    );

    if (SUCCEEDED(hr))
    {
        hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pD2DFactory);
    }

    if (SUCCEEDED(hr))
    {
        hr = DWriteCreateFactory(
            DWRITE_FACTORY_TYPE_SHARED,
            __uuidof(pDWriteFactory),
            reinterpret_cast<IUnknown**>(&pDWriteFactory)
        );
    }

    //
    // Create IWICBitmap and RT
    //

    static const UINT sc_bitmapWidth = 640;
    static const UINT sc_bitmapHeight = 480;
    if (SUCCEEDED(hr))
    {
        hr = pWICFactory->CreateBitmap(
            sc_bitmapWidth,
            sc_bitmapHeight,
            GUID_WICPixelFormat32bppBGR,
            WICBitmapCacheOnLoad,
            &pWICBitmap
        );
    }

    if (SUCCEEDED(hr))
    {
        hr = pD2DFactory->CreateWicBitmapRenderTarget(
            pWICBitmap,
            D2D1::RenderTargetProperties(),
            &pRT
        );
    }
    D2D1_BITMAP_PROPERTIES abracadabra;
    abracadabra.dpiX = 0.0f;
    abracadabra.dpiY = 0.0f;
    abracadabra.pixelFormat.format = DXGI_FORMAT_B8G8R8A8_UNORM;
    abracadabra.pixelFormat.alphaMode = D2D1_ALPHA_MODE_IGNORE;

    hr = pRT->CreateBitmap(D2D1::SizeU(x, y), bm.memory, (UINT32)(x * 4), abracadabra, &bitmap);
    if (SUCCEEDED(hr))
    {
        pRT->BeginDraw();
        pRT->Clear(D2D1::ColorF(D2D1::ColorF::White));
        pRT->DrawBitmap(bitmap);
        pRT->EndDraw();
    }
    if (SUCCEEDED(hr))
    {
        hr = pWICFactory->CreateStream(&pStream);
    }

    WICPixelFormatGUID format = GUID_WICPixelFormatDontCare;
    if (SUCCEEDED(hr))
    {
        static const WCHAR filename[] = L"C:\\Users\\Дима\\source\\repos\\Web\\Web\\wwwroot\\Bitmap.bmp";
        hr = pStream->InitializeFromFilename(filename, GENERIC_WRITE);
    }
    if (SUCCEEDED(hr))
    {
        hr = pWICFactory->CreateEncoder(GUID_ContainerFormatPng, NULL, &pEncoder);
    }
    if (SUCCEEDED(hr))
    {
        hr = pEncoder->Initialize(pStream, WICBitmapEncoderNoCache);
    }
    if (SUCCEEDED(hr))
    {
        hr = pEncoder->CreateNewFrame(&pFrameEncode, NULL);
    }
    if (SUCCEEDED(hr))
    {
        hr = pFrameEncode->Initialize(NULL);
    }
    if (SUCCEEDED(hr))
    {
        hr = pFrameEncode->SetSize(sc_bitmapWidth, sc_bitmapHeight);
    }
    if (SUCCEEDED(hr))
    {
        hr = pFrameEncode->SetPixelFormat(&format);
    }
    if (SUCCEEDED(hr))
    {
        hr = pFrameEncode->WriteSource(pWICBitmap, NULL);
    }
    if (SUCCEEDED(hr))
    {
        hr = pFrameEncode->Commit();
    }
    if (SUCCEEDED(hr))
    {
        hr = pEncoder->Commit();
    }

    SafeRelease(&pWICFactory);
    SafeRelease(&pD2DFactory);
    SafeRelease(&pDWriteFactory);
    SafeRelease(&pWICBitmap);
    SafeRelease(&pRT);
    SafeRelease(&pTextFormat);
    SafeRelease(&pPathGeometry);
    SafeRelease(&pSink);
    SafeRelease(&pGradientStops);
    SafeRelease(&pLGBrush);
    SafeRelease(&pBlackBrush);
    SafeRelease(&pEncoder);
    SafeRelease(&pFrameEncode);
    SafeRelease(&pStream);
}


