/**********************************************************************************************
*
*   Ngine - A 2D game engine.
*
*   Copyright 2020 NerdThings (Reece Mackie)
*
*   Licensed under the Apache License, Version 2.0 (the "License");
*   you may not use this file except in compliance with the License.
*   You may obtain a copy of the License at
*
*       http://www.apache.org/licenses/LICENSE-2.0
*
*   Unless required by applicable law or agreed to in writing, software
*   distributed under the License is distributed on an "AS IS" BASIS,
*   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*   See the License for the specific language governing permissions and
*   limitations under the License.
*
**********************************************************************************************/

#include "graphics/Image.hpp"

#include <cstring>
#include <stb_image.h>

namespace ngine::graphics {
    void Image::_create(unsigned char *pixelData, unsigned int width, unsigned int height, PixelFormat format) {
        // Bits per pixel
        int bpp = 0;
        if (format == UNCOMPRESSED_GRAYSCALE) bpp = 1;
        else if (format == UNCOMPRESSED_GRAY_ALPHA) bpp = 2;
        else if (format == UNCOMPRESSED_R8G8B8) bpp = 3;
        else if (format == UNCOMPRESSED_R8G8B8A8) bpp = 4;
        else throw std::runtime_error("Incompatible format.");

        // Copy pixel data
        PixelData = new unsigned char[width * height * bpp];
        memcpy(PixelData, pixelData, width * height * sizeof(unsigned char) * bpp);

        // Set fields
        Width = width;
        Height = height;
        Format = format;
        MipmapCount = 1;
    }

    Image::Image() {}

    Image::Image(Image &&old) {
        // Copy data.
        Width = old.Width;
        Height = old.Height;
        Format = old.Format;
        MipmapCount = old.MipmapCount;
        PixelData = old.PixelData;

        // Set pixel data to null so it is not deleted.
        old.PixelData = nullptr;
    }

    Image::Image(const filesystem::Path &path) {
        // Check format
        auto ext = path.getFileExtension();

        if (ext == "png"
            || ext == "bmp"
            || ext == "tga"
            || ext == "jpg"
            || ext == "gif"
            || ext == "pic"
            || ext == "psd") {
            // Open file
            auto file = filesystem::File(path);
            file.open(filesystem::FileOpenMode::Read, true);

            // Load data
            int width = 0, height = 0, bpp = 0;
            auto data = stbi_load_from_file(file.getFileHandle(), &width, &height, &bpp, 4);

            // Load image
            _create(data, width, height, UNCOMPRESSED_R8G8B8A8);

            // Close file
            stbi_image_free(data);
            file.close();
        }
    }

    Image::Image(unsigned char *pixelData, unsigned int width, unsigned int height, PixelFormat format) {
        _create(pixelData, width, height, format);
    }

    Image::~Image() {
        free();
    }

    bool Image::isValid() const {
        return Width > 0
                && Height > 0
                && PixelData != nullptr;
    }

    void Image::free() {
        // Removes image data from the container (across all copies of this image)
        Format = UNCOMPRESSED_GRAYSCALE;
        Height = 0;
        MipmapCount = 0;
        delete[] PixelData;
        PixelData = nullptr;
        Width = 0;
    }

}
