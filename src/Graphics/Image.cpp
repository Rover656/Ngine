/**********************************************************************************************
*
*   Ngine - The 2D game engine.
*
*   Copyright (C) 2019 NerdThings
*
*   LICENSE: Apache License 2.0
*   View: https://github.com/NerdThings/Ngine/blob/master/LICENSE
*
**********************************************************************************************/

#include "Image.h"

#include <stb_image.h>

namespace NerdThings::Ngine::Graphics {
    // Public Constructors

    Image::Image() {}

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
            file.Open(Filesystem::MODE_READ, true);

            // Load data
            int width = 0, height = 0, bpp = 0;
            // TODO: Work out why these cause havoc with the renderer
            //stbi_set_flip_vertically_on_load(true);
            auto data = stbi_load_from_file(file.GetFileHandle(), &width, &height, &bpp, 4);;
            //stbi_set_flip_vertically_on_load(false);

            // Load image
            *this = Image(data, width, height, UNCOMPRESSED_R8G8B8A8);

            // Close file
            stbi_image_free(data);
            file.Close();
        }
    }

    Image::Image(unsigned char *pixelData_, int width_, int height_, PixelFormat format_) {
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
        Mipmaps = 1;
    }

    // Public Methods

    bool Image::IsValid() const {
        return Width > 0
                && Height > 0
                && PixelData != nullptr;
    }

    Image *Image::LoadImage(const Filesystem::Path &path_) {
        return new Image(path_);
    }

    Image *Image::LoadPixels(unsigned char *pixelData_, int width_, int height_, PixelFormat format_) {
        return new Image(pixelData_, width_, height_, format_);
    }

    void Image::Unload() {
        // Removes image data from the container (across all copies of this image)
        Format = UNCOMPRESSED_GRAYSCALE;
        Height = 0;
        Mipmaps = 0;
        delete[] PixelData;
        PixelData = nullptr;
        Width = 0;
    }

}