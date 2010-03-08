#include "SpriteManager.h"

namespace m2d {

SpriteManager*
SpriteManager::m_pInstance = NULL;

void SpriteManager::Init()
{
	// string error = "ERROR";
	pSprite error_spr(new Sprite());
	sprites.insert(std::pair<string, pSprite>("ERROR", error_spr)); // Create the error sprite.

	error_color_array[0] = 1.0;
	error_color_array[1] = 17/255;
	error_color_array[2] = 231/255;
}
void SpriteManager::Deinit()
{
// Clear the map and multimap
    nodes.clear();
	sprites.clear();
}
void SpriteManager::AddResource(string file)
{
	pSprite sprite(new Sprite(file));
	if(sprite->GetName() != "ERROR")
	{
		if(sprites.find(sprite->GetName()) == sprites.end())
		{
			sprites.insert(std::pair<string, pSprite>(sprite->GetName(), sprite));
		}
		else
		{
			sprites.insert(std::pair<string, pSprite>(sprite->GetName(), sprite));
			Mob2DLog::Instance()->PushString("Sprite data overwritten.");
		}
	}
	else
		Mob2DLog::Instance()->PushString("Sprite load failed. Tried to overwrite default error resource.");
}
M2DNode SpriteManager::AddNode(string handle)
{
	if(sprites.find(handle) != sprites.end())
	{
		M2DNode node(new mob2d_node(sprites[handle]));
		nodes.insert(std::pair<string, M2DNode>(handle, node));
		return node;
	}
	else
	{
		M2DNode node(new mob2d_node(sprites["ERROR"]));
		Mob2DLog::Instance()->PushString("Mob2DNode set to error and left unmapped.");
		return node;
	}
}
void SpriteManager::MapNode(M2DNode node, string key)
{
    // Ain't I clever?
    if(sprites.find(key) == sprites.end())
        Mob2DLog::Instance()->PushString("ERROR! Could not map node to a nonexistant key.");
    else
        nodes.insert(std::pair<string, M2DNode>(key, node));
}
void SpriteManager::DeleteSprite(string sprite_name)
{
	// Delete the Sprite resource stored and reset all existing nodes pointing to it to the error sprite
	if(sprites.find(sprite_name) != sprites.end())
	{
        // all nodes now point to the error sprite and the sprite gets deallocated automatically by Boost.
		std::pair<Mob2DNodeIter, Mob2DNodeIter> range_pair = nodes.equal_range(sprite_name);
		for(Mob2DNodeIter i = range_pair.first; i != range_pair.second; i++)
		{
		    // reassign the node's internal pointer.
			(*i).second->ReassignToSprite(sprites["ERROR"]);
			(*i).second->SetAnimation("ERROR");
		}
		nodes.erase(sprite_name);
		sprites.erase(sprite_name);
	}
	else
        Mob2DLog::Instance()->PushString("Sprite resource not found. Can not delete.");
}
void SpriteManager::ClearNodes(string handle)
{
    if(nodes.find(handle) != nodes.end())
	{
        std::pair<Mob2DNodeIter, Mob2DNodeIter> range_pair = nodes.equal_range(handle);
		for(Mob2DNodeIter i = range_pair.first; i != range_pair.second; i++)
		{
		    // reassign the node's internal pointer.
			(*i).second->ReassignToSprite(sprites["ERROR"]);
			(*i).second->SetAnimation("ERROR");
		}
	    nodes.erase(handle);
	}
}
pSprite SpriteManager::GetSprite(string sprite_name)
{
	if(sprites.find(sprite_name) != sprites.end())
		return sprites[sprite_name];
	else
		return sprites["ERROR"];
}
void SpriteManager::Cleanup()
{
    // So easy a caveman can do it.
    nodes.erase("CLEANUP");
}

} // namespace
