#include "Detector.h"
#include "SRGBmodsPicoController.h"
#include "RGBController.h"
#include "RGBController_SRGBmodsPico.h"
#include <vector>
#include <hidapi/hidapi.h>

#define SRGBMODS_PICO_VID   0x16D0
#define SRGBMODS_PICO_PID   0x1123

/******************************************************************************************\
*                                                                                          *
*   DetectSRGBmodsControllers                                                              *
*                                                                                          *
*       Detect devices supported by the SRGBmods driver                                    *
*                                                                                          *
\******************************************************************************************/

void DetectSRGBmodsControllers(hid_device_info* info, const std::string& name)
{
    hid_device* dev = hid_open_path(info->path);    

    if(dev)
    {
        wchar_t product[128];
        hid_get_product_string(dev, product, 128);

        std::wstring product_str(product);
        
        /*-------------------------------------------------------------------------*\
        | Test the product string in case this USB ID is reused for other Pi Pico   |
        | projects                                                                  |
        \*-------------------------------------------------------------------------*/
        if(product_str == L"SRGBmods Pico LED Controller")
        {
            SRGBmodsPicoController*     controller     = new SRGBmodsPicoController(dev, info->path);
            RGBController_SRGBmodsPico* rgb_controller = new RGBController_SRGBmodsPico(controller);
            rgb_controller->name                       = name;

            ResourceManager::get()->RegisterRGBController(rgb_controller);
        }
    }
}   /* DetectSRGBmodsControllers() */

REGISTER_HID_DETECTOR("SRGBmods Pico LED Controller", DetectSRGBmodsControllers, SRGBMODS_PICO_VID, SRGBMODS_PICO_PID);
