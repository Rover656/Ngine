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

#include "Graphics/Image.hpp"

#include <stb_image.h>

namespace NerdThings::Ngine::Graphics {
    void Image::_create(unsigned char *pixelData_, unsigned int width_, unsigned int height_, PixelFormat format_) {
        // Bits per pixel
        int bpp = 0;
        if (format_ == UNCOMPRESSED_GRAYSCALE) bpp = 1;
        else if (format_ == UNCOMPRESSED_GRAY_ALPHA) bpp = 2;
        else if (format_ == UNCOMPRESSED_R8G8B8) bpp = 3;
        else if (format_ == UNCOMPRESSED_R8G8B8A8) bpp = 4;
        else throw std::runtime_error("Incompatible format.");

        // Copy pixel data
        PixelData = new unsigned char[width_*height_*bpp];
        memcpy(PixelData, pixelData_, width_*height_*sizeof(unsigned char)*bpp);

        // Set fields
        Width = width_;
        Height = height_;
        Format = format_;
        MipmapCount = 1;
    }

    Image::Image() {}

    Image::Image(Image &&old_) {
        // Copy data.
        Width = old_.Width;
        Height = old_.Height;
        Format = old_.Format;
        MipmapCount = old_.MipmapCount;
        PixelData = old_.PixelData;

        // Set pixel data to null so it is not deleted.
        old_.PixelData = nullptr;
    }

    Image::Image(const Filesystem::Path &path_) {
        // Check format
        auto ext = path_.GetFileExtension();

        if (ext == "png"
            || ext == "bmp"
            || ext == "tga"
            || ext == "jpg"
            || ext == "gif"
            || ext == "pic"
            || ext == "psd") {
            // Open file
            auto file = Filesystem::File(path_);
            file.Open(Filesystem::FileOpenMode::Read, true);

            // Load data
            int width = 0, height = 0, bpp = 0;
            auto data = stbi_load_from_file(file.GetFileHandle(), &width, &height, &bpp, 4);

            // Load image
            _create(data, width, height, UNCOMPRESSED_R8G8B8A8);

            // Close file
            stbi_image_free(data);
            file.Close();
        }
    }

    Image::Image(unsigned char *pixelData_, unsigned int width_, unsigned int height_, PixelFormat format_) {
        _create(pixelData_, width_, height_, format_);
    }

    bool Image::IsValid() const {
        return Width > 0
                && Height > 0
                && PixelData != nullptr;
    }

    void Image::Unload() {
        // Removes image data from the container (across all copies of this image)
        Format = UNCOMPRESSED_GRAYSCALE;
        Height = 0;
        MipmapCount = 0;
        delete[] PixelData;
        PixelData = nullptr;
        Width = 0;
    }

}