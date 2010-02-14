/*
Copyright (c) 2009 Peter "Corsix" Cawley

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef CORSIX_TH_TH_MAP_H_
#define CORSIX_TH_TH_MAP_H_
#include "th_gfx.h"

/*
    Object type enumeration uses same values as original TH does.
    See game string table section 39 for proof. Section 1 also has
    names in this order.
*/
enum THObjectType
{
    THOB_NoObject = 0,
    THOB_Desk = 1,
    THOB_Cabinet = 2,
    THOB_Door = 3,
    THOB_Bench = 4,
    THOB_Table = 5, // Not in game
    THOB_Chair = 6,
    THOB_DrinksMachine = 7,
    THOB_Bed = 8,
    THOB_Inflator = 9,
    THOB_PoolTable = 10,
    THOB_ReceptionDesk = 11,
    THOB_BTable = 12, // Not in game?
    THOB_Cardio = 13,
    THOB_Scanner = 14,
    THOB_ScannerConsole = 15,
    THOB_Screen = 16,
    THOB_LitterBomb = 17,
    THOB_Couch = 18,
    THOB_Sofa = 19,
    THOB_Crash = 20, // The trolley in general diagnosis
    THOB_TV = 21,
    THOB_Ultrascan = 22,
    THOB_DNAFixer = 23,
    THOB_CastRemover = 24,
    THOB_HairRestorer = 25,
    THOB_Slicer = 26,
    THOB_XRay = 27,
    THOB_RadiationShield = 28,
    THOB_XRayViewer = 29,
    THOB_OpTable = 30,
    THOB_Lamp = 31, // Not in game?
    THOB_Sink = 32,
    THOB_OpSink1 = 33,
    THOB_OpSink2 = 34,
    THOB_SurgeonScreen = 35,
    THOB_LectureChair = 36,
    THOB_Projector = 37,
    // 38 is unused
    THOB_Pharmacy = 39,
    THOB_Computer = 40,
    THOB_ChemicalMixer = 41,
    THOB_BloodMachine = 42,
    THOB_Extinguisher = 43,
    THOB_Radiator = 44,
    THOB_Plant = 45,
    THOB_Electro = 46,
    THOB_JellyVat = 47,
    THOB_Hell = 48,
    // 49 is unused
    THOB_Bin = 50,
    THOB_Loo = 51,
    THOB_DoubleDoor1 = 52,
    THOB_DoubleDoor2 = 53,
    THOB_DeconShower = 54,
    THOB_Autopsy = 55,
    THOB_Bookcase = 56,
    THOB_VideoGame = 57,
    THOB_EntranceLeftDoor = 58,
    THOB_EntranceRightDoor = 59,
    THOB_Skeleton = 60,
    THOB_ComfyChair = 61,
    // 62 through 255 are unused
};

enum THMapNodeFlags
{
    THMN_Passable   = 1 <<  0, //!< Pathfinding: Can walk on this tile
    THMN_CanTravelN = 1 <<  1, //!< Pathfinding: Can walk to the north
    THMN_CanTravelE = 1 <<  2, //!< Pathfinding: Can walk to the east
    THMN_CanTravelS = 1 <<  3, //!< Pathfinding: Can walk to the south
    THMN_CanTravelW = 1 <<  4, //!< Pathfinding: Can walk to the west
    THMN_Hospital   = 1 <<  5, //!< World: Tile is inside a hospital building
    THMN_Buildable  = 1 <<  6, //!< Player: Can build on this tile
    //! Pathfinding: Normally can walk on this tile, but can't due to blueprint
    THMN_PassableIfNotForBlueprint = 1 << 7,
    THMN_PassableIfNotForBlueprint_ShiftDelta = 7,
    THMN_Room       = 1 <<  8, //!< World: Tile is inside a room
    THMN_ShadowHalf = 1 <<  9, //!< Rendering: Put block 75 over floor
    THMN_ShadowFull = 1 << 10, //!< Rendering: Put block 74 over floor
    THMN_ShadowWall = 1 << 11, //!< Rendering: Put block 156 over east wall
    THMN_DoorNorth  = 1 << 12, //!< World: Door on north wall of tile
    THMN_DoorWest   = 1 << 13, //!< World: Door on west wall of tile
    THMN_DoNotIdle  = 1 << 14, //!< World: Humanoids should not idle on tile
    THMN_TallNorth  = 1 << 15, //!< Shadows: Wall-like object on north wall
    THMN_TallWest   = 1 << 16, //!< Shadows: Wall-like object on west wall

    // NB: Bits 24 through 31 reserved for object type (that being one of the
    // THObjectType values)
};

struct THMapNode : public THLinkList
{
    THMapNode();

    // Linked list for entities rendered at this node
    // THLinkList::pPrev (will always be NULL)
    // THLinkList::pNext

    // Linked list for entities rendered in an early (right-to-left) pass
    THLinkList oEarlyEntities;

    // Block tiles for rendering
    // For each tile, the lower byte is the index in the sprite sheet, and the
    // upper byte is for the drawing flags.
    // Layer 0 is for the floor
    // Layer 1 is for the north wall
    // Layer 2 is for the west wall
    // Layer 3 is for the UI
    // NB: In Lua, layers are numbered 1 - 4 rather than 0 - 3
    uint16_t iBlock[4];

    // Parcels (plots) of land have an ID, with each tile in the plot having
    // that ID. Parcel 0 is the outside.
    uint16_t iParcelId;

    // Rooms have an ID, with room #0 being the corridor (and the outside).
    uint16_t iRoomId;

    // Flags for information (lower 24 bits) and object type (top 8 bits)
    // See THMapNodeFlags for lower 24 bits, and THObjectType for top 8.
    // The point of storing the object type here is to allow pathfinding code
    // to use object types as pathfinding goals.
    uint32_t iFlags;
};

class THSpriteSheet;

//! Prototype for object callbacks from THMap::loadFromTHFile
/*!
    The callback function will receive 5 arguments:
      * The opaque pointer passed to THMap::loadFromTHFile (pCallbackToken).
      * The tile X/Y position of the object.
      * The object type.
      * The object flags present in the map data. The meaning of this
        value is left unspecified.
*/
typedef void (*THMapLoadObjectCallback_t)(void*, int, int, THObjectType, uint8_t);

class THMap
{
public:
    THMap();
    ~THMap();

    bool setSize(int iWidth, int iHeight);
    bool loadFromTHFile(const unsigned char* pData, size_t iDataLength,
                        THMapLoadObjectCallback_t fnObjectCallback,
                        void* pCallbackToken);

    //! Set the sprite sheet to be used for drawing the map
    /*!
        The sprites for map floor tiles, wall tiles, and map decorators
        all come from the given sheet.
    */
    void setBlockSheet(THSpriteSheet* pSheet);

    //! Set the draw flags on all wall blocks
    /*!
        This is typically called with THDF_Alpha50 to draw walls transparently,
        or with 0 to draw them opaque again.
    */
    void setAllWallDrawFlags(unsigned char iFlags);

    void updatePathfinding();
    void updateShadows();

    //! Get the map width (in tiles)
    inline int getWidth()  const {return m_iWidth;}

    //! Get the map height (in tiles)
    inline int getHeight() const {return m_iHeight;}

    //! Draw the map (and any attached animations)
    /*!
        Draws the world pixel rectangle (iScreenX, iScreenY, iWidth, iHeight)
        to the rectangle (iCanvasX, iCanvasY, iWidth, iHeight) on pCanvas. Note
        that world pixel co-ordinates are also known as absolute screen
        co-ordinates - they are not world (tile) co-ordinates, nor (relative)
        screen co-ordinates.
    */
    void draw(THRenderTarget* pCanvas, int iScreenX, int iScreenY, int iWidth,
              int iHeight, int iCanvasX, int iCanvasY) const;

    //! Perform a hit-test against the animations attached to the map
    /*!
        If there is an animation at world pixel co-ordinates (iTestX, iTestY),
        then it is returned. Otherwise NULL is returned.
        To perform a hit-test using world (tile) co-ordinates, get the node
        itself and query the top 8 bits of THMapNode::iFlags, or traverse the
        node's animation lists.
    */
    THDrawable* hitTest(int iTestX, int iTestY) const;

    // When using the unchecked versions, the map co-ordinates MUST be valid.
    // When using the normal versions, NULL is returned for invalid co-ords.
          THMapNode* getNode(int iX, int iY);
    const THMapNode* getNode(int iX, int iY) const;
    const THMapNode* getOriginalNode(int iX, int iY) const;
          THMapNode* getNodeUnchecked(int iX, int iY);
    const THMapNode* getNodeUnchecked(int iX, int iY) const;
    const THMapNode* getOriginalNodeUnchecked(int iX, int iY) const;

    //! Convert world (tile) co-ordinates to absolute screen co-ordinates
    template <typename T>
    static inline void worldToScreen(T& x, T& y)
    {
        T x_(x);
        x = (T)32 * (x_ - y);
        y = (T)16 * (x_ + y);
    }

    //! Convert absolute screen co-ordinates to world (tile) co-ordinates
    template <typename T>
    static inline void screenToWorld(T& x, T& y)
    {
        T x_(x);
        x = y / (T)32 + x_ / (T)64;
        y = y / (T)32 - x_ / (T)64;
    }

    void persist(LuaPersistWriter *pWriter) const;
    void depersist(LuaPersistReader *pReader);

protected:
    THDrawable* _hitTestDrawables(THLinkList* pListStart, int iXs, int iYs,
                                  int iTestX, int iTestY) const;

    THMapNode* m_pCells;
    THMapNode* m_pOriginalCells; // Cells at map load time, before any changes
    THSpriteSheet* m_pBlocks;
    int m_iWidth;
    int m_iHeight;
};

enum eTHMapScanlineIteratorDirection
{
    ScanlineForward = 2,
    ScanlineBackward = 0,
};

//! Utility class for iterating over map nodes within a screen rectangle
/*!
    To easily iterate over the map nodes which might draw something within a
    certain rectangle of screen space, an instance of this class can be used.

    By default, it iterates by scanline, top-to-bottom, and then left-to-right
    within each scanline. Alternatively, by passing ScanlineBackward to the
    constructor, it will iterate bottom-to-top. Within a scanline, to visit
    nodes right-to-left, wait until isLastOnScanline() returns true, then use
    an instance of THMapScanlineIterator.
*/
class THMapNodeIterator
{
public:
    /*!
        @arg pMap The map whose nodes should be iterated
        @arg iScreenX The X co-ordinate of the top-left corner of the
            screen-space rectangle to iterate.
        @arg iScreenY The Y co-ordinate of the top-left corner of the
            screen-space rectangle to iterate.
        @arg iWidth The width of the screen-space rectangle to iterate.
        @arg iHeight The width of the screen-space rectangle to iterate.
        @arg eScanlineDirection The direction in which to iterate scanlines;
            ScanlineForward for top-to-bottom, ScanlineBackward for bottom-to-top.
    */
    THMapNodeIterator(const THMap *pMap, int iScreenX, int iScreenY,
                      int iWidth, int iHeight,
                      eTHMapScanlineIteratorDirection eScanlineDirection = ScanlineForward);

    //! Returns false iff the iterator has exhausted its nodes
    inline operator bool () const {return m_pNode != NULL;}

    //! Advances the iterator to the next node
    inline THMapNodeIterator& operator ++ ();

    //! Accessor for the current node
    inline const THMapNode* operator -> () const {return m_pNode;}

    //! Get the X position of the node relative to the top-left corner of the screen-space rectangle
    inline int x() const {return m_iXs;}

    //! Get the Y position of the node relative to the top-left corner of the screen-space rectangle
    inline int y() const {return m_iYs;}

    //! Returns true iff the next node will be on a different scanline
    /*!
        To visit a scanline in right-to-left order, or to revisit a scanline,
        wait until this method returns true, then use a THMapScanlineIterator.
    */
    inline bool isLastOnScanline() const;
  
protected:
    // Maximum extents of the visible parts of a node (pixel distances relative
    // to the top-most corner of an isometric cell)
    // If set too low, things will disappear when near the screen edge
    // If set too high, rendering will slow down
    static const int ms_iMarginTop = 32;
    static const int ms_iMarginLeft = 56;
    static const int ms_iMarginRight = 56;
    static const int ms_iMarginBottom = 70;

    friend class THMapScanlineIterator;

    const THMapNode* m_pNode;
    const THMap* m_pMap;
    int m_iXs;
    int m_iYs;
    const int m_iScreenX;
    const int m_iScreenY;
    const int m_iScreenWidth;
    const int m_iScreenHeight;
    int m_iBaseX;
    int m_iBaseY;
    int m_iX;
    int m_iY;
    int m_iScanlineCount;
    eTHMapScanlineIteratorDirection m_eDirection;

    void _advanceUntilVisible();
};

//! Utility class for re-iterating a scanline visited by a THMapNodeIterator
class THMapScanlineIterator
{
public:
    /*!
        @arg itrNodes A node iterator which has reached the end of a scanline
        @arg eDirection The direction in which to iterate the scanline;
            ScanlineForward for left-to-right, ScanlineBackward for right-to-left.
        @arg iXOffset If given, values returned by x() will be offset by this.
        @arg iYOffset If given, values returned by y() will be offset by this.
    */
    THMapScanlineIterator(const THMapNodeIterator& itrNodes,
                          eTHMapScanlineIteratorDirection eDirection,
                          int iXOffset = 0, int iYOffset = 0);

    inline operator bool () const {return m_pNode != m_pNodeEnd;}
    inline THMapScanlineIterator& operator ++ ();

    inline const THMapNode* operator -> () const {return m_pNode;}
    inline int x() const {return m_iXs;}
    inline int y() const {return m_iYs;}

protected:
    const THMapNode* m_pNode;
    const THMapNode* m_pNodeEnd;
    const int m_iNodeStep;
    const int m_iXStep;
    int m_iXs;
    int m_iYs;
};

#endif // CORSIX_TH_TH_MAP_H_
