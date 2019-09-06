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
            int bpp = 0;
            // TODO: Work out why these cause havoc with the renderer
            //stbi_set_flip_vertically_on_load(true);
            img._PixelDataContainer->PixelData = stbi_load_from_file(file.GetFileHandle(), &img._Width, &img._Height, &bpp, 4);
            //stbi_set_flip_vertically_on_load(false);

            // Close file
            file.Close();

            // TODO: stbi_image_free()

            // Set mipmaps
            img._Mipmaps = 1;

            // Get format
            img._Format = UNCOMPRESSED_R8G8B8A8;
        }

        return img;
    }

    // InternalDataContainer: Destructor

    TImage::InternalDataContainer::~InternalDataContainer() {
        // Free pixel data
        stbi_image_free(PixelData);
    }
}