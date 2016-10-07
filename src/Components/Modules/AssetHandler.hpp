namespace Components
{
	class AssetHandler : public Component
	{
	public:
		class IAsset
		{
		public:
			virtual ~IAsset() {};
			virtual Game::XAssetType GetType() { return Game::XAssetType::ASSET_TYPE_INVALID; };
			virtual void Mark(Game::XAssetHeader /*header*/, ZoneBuilder::Zone* /*builder*/) { /*ErrorTypeNotSupported(this);*/ };
			virtual void Save(Game::XAssetHeader /*header*/, ZoneBuilder::Zone* /*builder*/) { /*ErrorTypeNotSupported(this);*/ };
			virtual void Dump(Game::XAssetHeader /*header*/) { /*ErrorTypeNotSupported(this);*/ };
			virtual void Load(Game::XAssetHeader* /*header*/, std::string name, ZoneBuilder::Zone* /*builder*/) { /*ErrorTypeNotSupported(this);*/ };
		};

		typedef Game::XAssetHeader(Callback)(Game::XAssetType type, std::string name);
		typedef void(RestrictCallback)(Game::XAssetType type, Game::XAssetHeader asset, std::string name, bool* restrict);

		AssetHandler();
		~AssetHandler();

#if defined(DEBUG) || defined(FORCE_UNIT_TESTS)
		const char* GetName() { return "AssetHandler"; };
#endif

		static void OnFind(Game::XAssetType type, Callback* callback);
		static void OnLoad(RestrictCallback* callback);

		static void ClearRelocations();
		static void Relocate(void* start, void* to, DWORD size = 4);

		static void Relocate(DWORD start, DWORD size, DWORD to) {
			Relocate((void*)start, (void*)to, size);
		}

		static void ZoneSave(Game::XAsset asset, ZoneBuilder::Zone* builder);
		static void ZoneMark(Game::XAsset asset, ZoneBuilder::Zone* builder);

		static Game::XAssetHeader FindOriginalAsset(Game::XAssetType type, const char* filename);
		static Game::XAssetHeader FindAssetForZone(Game::XAssetType type, std::string filename, ZoneBuilder::Zone* builder);

		static void ClearTemporaryAssets();
		static void StoreTemporaryAsset(Game::XAssetType type, Game::XAssetHeader asset);

	private:
		static bool BypassState;

		static std::map<std::string, Game::XAssetHeader> TemporaryAssets[Game::XAssetType::ASSET_TYPE_COUNT];

		static std::map<Game::XAssetType, IAsset*> AssetInterfaces;
		static std::map<Game::XAssetType, wink::slot<Callback>> TypeCallbacks;
		static wink::signal<wink::slot<RestrictCallback>> RestrictSignal;

		static std::map<void*, void*> Relocations;

		static std::vector<std::pair<Game::XAssetType, std::string>> EmptyAssets;

		static void RegisterInterface(IAsset* iAsset);

		static Game::XAssetHeader FindAsset(Game::XAssetType type, const char* filename);
		static bool IsAssetEligible(Game::XAssetType type, Game::XAssetHeader* asset);
		static void FindAssetStub();
		static void AddAssetStub();

		static void OffsetToAlias(Utils::Stream::Offset* offset);

		static void StoreEmptyAsset(Game::XAssetType type, const char* name);
		static void StoreEmptyAssetStub();
	};
}

#include "AssetInterfaces\IXModel.hpp"
#include "AssetInterfaces\IMapEnts.hpp"
#include "AssetInterfaces\IRawFile.hpp"
#include "AssetInterfaces\IGfxImage.hpp"
#include "AssetInterfaces\IMaterial.hpp"
#include "AssetInterfaces\IPhysPreset.hpp"
#include "AssetInterfaces\IXAnimParts.hpp"
#include "AssetInterfaces\IPhysCollmap.hpp"
#include "AssetInterfaces\IStringTable.hpp"
#include "AssetInterfaces\IXModelSurfs.hpp"
#include "AssetInterfaces\ILocalizedEntry.hpp"
#include "AssetInterfaces\IMaterialPixelShader.hpp"
#include "AssetInterfaces\IMaterialTechniqueSet.hpp"
#include "AssetInterfaces\IMaterialVertexShader.hpp"
#include "AssetInterfaces\IStructuredDataDefSet.hpp"
#include "AssetInterfaces\IMaterialVertexDeclaration.hpp"
