#include "Sprite.h"S

Sprite::Sprite()
{
	error_flag = true;
	name = "ERROR";

	pAnimation anim(new Animation);

	Frame frame;
	frame.vertex_array[0] = frame.vertex_array[1] = frame.vertex_array[2] = frame.vertex_array[3] =
	frame.vertex_array[4] = frame.vertex_array[5] = frame.vertex_array[6] = frame.vertex_array[7] = 0;

	frame.texture_coords[0] = frame.texture_coords[1] = frame.texture_coords[2] = frame.texture_coords[3] =
	frame.texture_coords[4] = frame.texture_coords[5] = frame.texture_coords[6] = frame.texture_coords[7] = 0.0f;

	anim->frames.push_back(frame);
	anim->SetAABBData();
	anim->SetBoundingCircleData();
	animations.insert(std::pair<string, pAnimation>("ERROR", anim));
}
Sprite::Sprite(string file)
{
	error_flag = false;

	// Default first animation that nodes will be able to point to when they are first initalized.
	pAnimation anim(new Animation);
	Frame frame;
	frame.vertex_array[0] = frame.vertex_array[1] = frame.vertex_array[2] = frame.vertex_array[3] =
	frame.vertex_array[4] = frame.vertex_array[5] = frame.vertex_array[6] = frame.vertex_array[7]= 0;
	frame.texture_coords[0] = frame.texture_coords[1] = frame.texture_coords[2] = frame.texture_coords[3] =
	frame.texture_coords[4] = frame.texture_coords[5] = frame.texture_coords[6] = frame.texture_coords[7] = 0.0f;
	anim->frames.push_back(frame);
	anim->SetAABBData();
	anim->SetBoundingCircleData();
	animations.insert(std::pair<string, pAnimation>("ERROR", anim));
    // End default animation

	string file_to_load;

	TiXmlDocument doc(file);
	bool loaded = doc.LoadFile();

	TiXmlHandle sheet_dec(&doc);

	TiXmlElement* root;
	TiXmlElement* properties_element;
	if(!doc.Error()) {
		if(loaded) {
			root = sheet_dec.FirstChildElement("spritesheet").ToElement();
			properties_element = root->FirstChildElement("image_properties");

			if(root && properties_element) {
				file_to_load = root->Attribute("sheet_source");
				name = root->Attribute("name"); // The name parameter.

				properties_element->QueryIntAttribute("width", &image_width);
				properties_element->QueryIntAttribute("height", &image_height);

				LoadAnimations(root);

				file_to_load = root->Attribute("sheet_source");

				image_handle = SOIL_load_OGL_texture(file_to_load.c_str(),
													SOIL_LOAD_AUTO,
													SOIL_CREATE_NEW_ID,
													SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y );

				if(0 == image_handle)
					DARSLog::Instance()->PushString("Image file could not be loaded for "+name+".\n");
                DARSLog::Instance()->PushString("Spritesheet successfuly loaded.\n");
			}
			else {
				if(!root)
					DARSLog::Instance()->PushString("Sprite sheet declaration invalid. No <spritesheet>.\n");
				if(!properties_element)
					DARSLog::Instance()->PushString("Sprite sheet declaration invalid. No <image_properties>.\n");
				error_flag = true;
			}
		}
		else {
			DARSLog::Instance()->PushString("Could not load definition file for sprite.\n");
			error_flag = true;
		}
	}
	else {
		DARSLog::Instance()->PushString("The document has errors in it. Fix the errors and check your tags!\n");
		error_flag = true;
	}
}
// if a spritesheet definition does not have at least one animation element with at least one frame element, then it is invalid.
void Sprite::LoadAnimations(TiXmlElement* root)
{
	// Scan for the first animation element.
	TiXmlElement* anim_element = root->FirstChildElement("animation");
	if(anim_element) {
		// outer loop. load the animation only after the frames for the animation are loaded.
		for(anim_element; anim_element; anim_element = anim_element->NextSiblingElement("animation")) {
			// Init an animation instance
			pAnimation anim(new Animation);
			string anim_name;

			TiXmlElement* frame_element = anim_element->FirstChildElement("frame");

			// loop through the frames
			if(frame_element) {
				// inner loop. loop through all of the frame elements and push them into the animation.
				for(frame_element; frame_element; frame_element = frame_element->NextSiblingElement("frame"))
					LoadAnimationFrame(frame_element, anim);
			}
			else {
				DARSLog::Instance()->PushString("No frames for this spritesheet animation.");
			}
			anim_name = anim_element->Attribute("name");
			anim->SetAABBData();
			anim->SetBoundingCircleData();
			animations[anim_name] = anim;
		}
	}
	else {
		DARSLog::Instance()->PushString("No animations for this spritesheet. Sheet marked as invalid.");
		error_flag = true;
	}
//	DARSLog::Instance()->PushString("Spritesheet loaded.\n");
}
void Sprite::LoadAnimationFrame(TiXmlElement* frame_element, pAnimation anim)
{
    TiXmlElement* tl_elem = frame_element->FirstChildElement("top_left");

    int top_left_x;
	tl_elem->QueryIntAttribute("x", &top_left_x);

	int top_left_y;
	tl_elem->QueryIntAttribute("y", &top_left_y);

    TiXmlElement* br_elem = frame_element->FirstChildElement("bottom_right");

    int bottom_right_x;
	br_elem->QueryIntAttribute("x", &bottom_right_x);

	int bottom_right_y;
	br_elem->QueryIntAttribute("y", &bottom_right_y);

	anim->frames.push_back(SetFrameData(top_left_x, top_left_y, bottom_right_x, bottom_right_y));
}

Frame Sprite::SetFrameData(int tl_x, int tl_y, int br_x, int br_y)
{
    Frame frame;

    GLfloat width, height;
	width = (GLfloat)((br_x - tl_x) / 2);
	height = (GLfloat)((br_y - tl_y) / 2);

	frame.vertex_array[0] = -width; frame.vertex_array[1] = height; // lower left
	frame.vertex_array[2] = width; frame.vertex_array[3] = height; // lower right
	frame.vertex_array[4] = width; frame.vertex_array[5] = -height; // upper right
	frame.vertex_array[6] = -width; frame.vertex_array[7] = -height; // upper left

	// bottom left
	frame.texture_coords[0] = (GLfloat)((GLfloat)tl_x/(GLfloat)image_width);
	frame.texture_coords[1] = (GLfloat)(1.0f-((GLfloat)br_y/(GLfloat)image_height));

	// bottom right
	frame.texture_coords[2] = (GLfloat)((GLfloat)br_x/(GLfloat)image_width);
	frame.texture_coords[3] = (GLfloat)(1.0f-((GLfloat)br_y/(GLfloat)image_height));

	// top right
    frame.texture_coords[4] = (GLfloat)((GLfloat)br_x/(GLfloat)image_width);
	frame.texture_coords[5] = (GLfloat)(1.0f-((GLfloat)tl_y/(GLfloat)image_height));

	// top left
	frame.texture_coords[6] = (GLfloat)((GLfloat)tl_x/(GLfloat)image_width);
	frame.texture_coords[7] = (GLfloat)(1.0f-((GLfloat)tl_y/(GLfloat)image_height));

    // DERRRRRRRRRRRRRRRRRRRRRRRRRRP! I NEVER RETURNED IT! A testimate to my occasional short-sightendess.
    return frame;
}
// upper left - -
// upper right + -
// lower right + +
// lower left - +


// old code
/*
	TiXmlElement* root;
	TiXmlElement* properties_element;
	TiXmlElement* frame_props_element;
	TiXmlElement* animations_element;

	TiXmlDocument doc(file);

	bool loaded = doc.LoadFile();

	TiXmlHandle sheet_dec(&doc);

	// Load the XML documents elements.
	if(loaded)
	{ root = sheet_dec.FirstChildElement("spritesheet").ToElement(); // First declaration
		if(root)
		{ properties_element = root->FirstChildElement("image_properties");
			if(properties_element)
			{ frame_props_element = root->FirstChildElement("frame_properties");
				if(frame_props_element)
				{ animations_element = root->FirstChildElement("animations");
					// All of the elements have successfuly loaded. Now store the properties contained within.
					if(animations_element)
					{
						file_to_load = root->Attribute("sheet_source");
						name = root->Attribute("name"); // The name parameter.
						properties_element->QueryIntAttribute("width", &image_width);
						properties_element->QueryIntAttribute("height", &image_height);
						frame_props_element->QueryIntAttribute("width", &frame_width);
						frame_props_element->QueryIntAttribute("height", &frame_height);

						// Now loop through the animations.
						TiXmlElement* next; TiXmlElement* child;
						child = animations_element->FirstChildElement("anim");

						int i;
						int push_in;
						animations_element->QueryIntAttribute("num_anims", &i);

						for(int is = 0; is < i; is++)
						{
							child->QueryIntAttribute("frames", &push_in);
							animations.push_back(push_in);
							if(child->NextSiblingElement("anim") != NULL)
							{
								next = child->NextSiblingElement("anim");
								child = next;
							}
						}

						image_handle = SOIL_load_OGL_texture(file_to_load.c_str(),
															 SOIL_LOAD_AUTO,
															 SOIL_CREATE_NEW_ID,
															 SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y );
						if(0 == image_handle)
						{
							error_flag = true;
							DARSLog::Instance()->PushString("Sprite sheet image data could not be loaded. This sprite will return an error.\n");
						}
						else
						{
							DARSLog::Instance()->PushString(("Sprite " + name + " successfuly loaded.\n"));
						}
						SetVertArray();
					}else {
						DARSLog::Instance()->PushString("No <animations> tag. This sprite will not be animated.\n");
						file_to_load = root->Attribute("sheet_source");
						name = root->Attribute("name"); // The name parameter.
						properties_element->QueryIntAttribute("width", &image_width);
						properties_element->QueryIntAttribute("height", &image_height);
						frame_props_element->QueryIntAttribute("width", &frame_width);
						frame_props_element->QueryIntAttribute("height", &frame_height);

						image_handle = SOIL_load_OGL_texture(file_to_load.c_str(),
															 SOIL_LOAD_AUTO,
															 SOIL_CREATE_NEW_ID,
															 SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y );
						if(0 == image_handle )
						{
							DARSLog::Instance()->PushString("Sprite sheet image data could not be loaded. This sprite will return an error.\n");
							error_flag = true;
						}
						else
						{
							DARSLog::Instance()->PushString(("Sprite " + name + " successfuly loaded.\n"));
						}
						SetVertArray();
						// Do not loop through the animation branch.
					}
				}else {
					DARSLog::Instance()->PushString("Sprite sheet declaration invalid. No <frame_properties>.\n");
					error_flag = true;
				}
			}else {
				DARSLog::Instance()->PushString("Sprite sheet declaration invalid. No <image_properties>.\n");
				error_flag = true;
			}
		}else {
			DARSLog::Instance()->PushString("Sprite sheet declaration invalid. No <spritesheet>.\n");
			error_flag = true;
		}
	}else {
		DARSLog::Instance()->PushString(("Could not load definition file for sprite: " + name + "\n"));
		error_flag = true;
	}
*/
