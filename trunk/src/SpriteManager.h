#ifndef SPRITEMANAGER_H
#define SPRITEMANAGER_H

// #include "FileManager.h"
#include "Mob2DCommon.h"
#include "Sprite.h"
#include "mob2d_node.h"

class SpriteManager;

/**
SpriteManager:
This class holds and caches all available sprite resources and also caches all of the nodes in a multimap. The
nice thing about the std::multimap is that automatically sorts its data, so if the Nodes are mapped in this class
according to the name of the sprite resource they are associated with, all that is needed to cut down on texture
binds is to iterate through the map like normal and simply check to make sure we would not be binding a texture
that was already bound.
\n\n
Nodes versus Sprites -
Sprites are only resources stored by the engine. They don't do much besides provide data. Nodes however are
updated and read by the engine and provide indirect access to the sprite resources.
*/
class SpriteManager
{
	struct strsrt
	{
		bool operator()(const string str1, const string str2) const
		{ return strcmp(str1.c_str(), str2.c_str()) < 0; }
	};
	public:
		static SpriteManager* Instance()
		{ if(!m_pInstance) m_pInstance = new SpriteManager; return m_pInstance; }

		void Init();
		void Deinit();

    /// Creates a new sprite, inserts it into the map, and delegates responsibility of loading the data to the
    /// Sprite instance. Sprite is hashed based on the name defined in its definition.
		void AddResource(string file);

    /// Adds an instance node to the internal multimap and returns a copy of it.
		Mob2DNode AddNode(string handle);

    /// Releases a texture and flags all nodes using the resource to return the error sprite and sets all
    /// nodes using this resource to display an error texture.
		void DeleteSprite(string sprite_name);

    /// Clears all the instance nodes using the given resource.
		void ClearNodes(string handle);

	/// Returns a pointer to a sprite. Used in lookup routines.
		pSprite GetSprite(string sprite_name);

    /// Clears all instance nodes that have no associated resource. Essentially a garbage collection function.
		void Cleanup();

    /// Used to map an already existing node so that it can be rendered. The function will check against the
    /// sprites map to make sure that the node is being mapped to an existing key to avoid inefficient rendering.
		void MapNode(Mob2DNode node, string key);

    /// An array used to render error instances.
		GLfloat error_color_array[3];

	protected:
		SpriteManager(){};
		virtual ~SpriteManager(){};
		SpriteManager(SpriteManager const&){};
		SpriteManager& operator=(SpriteManager const&){};
		static SpriteManager* m_pInstance;

    /// std::map<string, pSprite> of cached sprite resources.
		std::map<string, pSprite, strsrt> sprites; // boost::unordered_map<string, pSprite>

    /// std::multimap<string, Mob2DNode> of all of the available rendering nodes. This allows for sorting
    /// of the sprite nodes and leads to fewer texture changes. It also allows iteration through a
    /// specific range of nodes to flag them for deletion for the purpose of garbage collection.
		std::multimap<string, Mob2DNode, strsrt> nodes;

		friend class Mob2DRenderer;
};
typedef std::multimap<string, Mob2DNode>::iterator Mob2DNodeIter;

#endif
