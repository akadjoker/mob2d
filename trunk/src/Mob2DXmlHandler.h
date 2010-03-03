#ifndef DARSXMLHANDLER_H
#define DARSXMLHANDLER_H

#include "DARSCommon.h"
#include "Animation.h"
#include "xml/tinyxml.h"
#include <stack>

/**
@class DARSXmlHandler
Class meant to abstract the current verbose XML parsing done in the Sprite class.
*/

class DARSXmlHandler
{
    public:
        DARSXmlHandler() { root_element = NULL; };
        virtual ~DARSXmlHandler() { delete root_element; };

    /// Sets root element.
        bool LoadDocument(string file);

    /// Must be implimented by derived class.
        virtual uint ParseDocument() = 0;

    protected:
        TiXmlDocument document;
        TiXmlElement* root_element;

};

class DARSSpriteSheetLoader : public DARSXmlHandler
{
    public:
        DARSSpriteSheetLoader() {};
        ~DARSSpriteSheetLoader(){};

    /// Get everything from the document, returning a specific value when an error occurs. returns 1 on success.
        uint ParseDocument();

        bool LoadName();
        bool LoadImageProperties();
        bool LoadImageFile();
        bool LoadAnimations();


        bool LoadAnimationFrame(TiXmlElement* frame_element, pAnimation anim);
        Frame SetFrameData(int tl_x, int tl_y, int br_x, int br_y);

        void ReturnValues(string *name, int *image_width, int *image_height, GLuint *image_handle)
        {
            *name = this->name;
            *image_width = this->image_width;
            *image_height = this->image_height;
            *image_handle = this->image_handle;
        }

        std::vector<std::pair<string, pAnimation> > animations;

    protected:
        string name;
        int image_width;
        int image_height;
        GLuint image_handle;
        // std::map<string, pAnimation, strsrt> animations;

        void CreateErrorAnimation();
};

#endif // DARSXMLHANDLER_H
