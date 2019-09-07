#include "Image.h"

#include <stb_image.h>

namespace NerdThings::Ngine::Graphics {
    // Public Constructors

    TImage::TImage() {
        // Create container
        _PixelDataContainer = std::make_shared<InternalDataContainer>();
    }

    // Public Methods

    EPixelFormat TImage::GetFormat() {
        return _Format;
    }

    int TImage::GetHeight() {
        return _Height;
    }

    int TImage::GetMipmapCount() {
        return _Mipmaps;
    }

    void *TImage::GetPixelData() {
        return _PixelDataContainer->PixelData;
    }

    int TImage::GetWidth() {
        return _Width;
    }

    TImage TImage::LoadImage(const Filesystem::TPath &path_) {
        // Null image
        TImage img;

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
            auto file = Filesystem::TFile(path_);
            file.Open(Filesystem::MODE_READ, true);

            // Load data
            int width = 0, height = 0, bpp = 0;
            // TODO: Work out why these cause havoc with the renderer
            //stbi_set_flip_vertically_on_load(true);
            auto data = stbi_load_from_file(file.GetFileHandle(), &width, &height, &bpp, 4);;
            //stbi_set_flip_vertically_on_load(false);

            // Load image
            img = LoadPixels(data, width, height, UNCOMPRESSED_R8G8B8A8);

            // Close file
            stbi_image_free(data);
            file.Close();
        }

        return img;
    }

    TImage TImage::LoadPixels(unsigned char *pixelData_, int width_, int height_, EPixelFormat format_) {
        // The image
        TImage img;

        // Bits per pixel
        int bpp = 0;
        if (format_ == UNCOMPRESSED_GRAYSCALE) bpp = 1;
        else if (format_ == UNCOMPRESSED_GRAY_ALPHA) bpp = 2;
        else if (format_ == UNCOMPRESSED_R8G8B8) bpp = 3;
        else if (format_ == UNCOMPRESSED_R8G8B8A8) bpp = 4;
        else throw std::runtime_error("Incompatible format.");

        // Copy pixel data
        img._PixelDataContainer->PixelData = new unsigned char[width_*height_*bpp];
        memcpy(img._PixelDataContainer->PixelData, pixelData_, width_*height_*sizeof(unsigned char)*bpp);

        // Set fields
        img._Width = width_;
        img._Height = height_;
        img._Format = format_;
        img._Mipmaps = 1;

        return img;
    }

    // InternalDataContainer: Destructor

    TImage::InternalDataContainer::~InternalDataContainer() {
        // Free pixel data
        delete[] PixelData;
    }
}