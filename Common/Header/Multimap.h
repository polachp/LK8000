/*
   LK8000 Tactical Flight Computer -  WWW.LK8000.IT
   Released under GNU/GPL License v.2
   See CREDITS.TXT file for authors and copyrights

   $Id$
*/

#ifndef MULTIMAP_H
#define MULTIMAP_H

#define NUMBER_OF_MULTIMAPS	MP_TOP

extern int	Current_Multimap_SizeY;
extern RECT	Current_Multimap_TopRect;
extern double	Current_Multimap_TopAngle;
extern double	Current_Multimap_TopZoom;
extern POINT	Current_Multimap_TopOrig;
extern bool	Current_Multimap_IsActive;

bool IsMultiMap(void);
bool IsMultiMapNoMain(void);
bool IsMultiMapCustom(void);
bool IsMultiMapShared(void);
bool IsMultiMapSharedNoMain(void);
void MultiMapSound(void);

void ToggleMultimapTerrain(void);
void ToggleMultimapAirspace(void);
void ToggleMultimapTopology(void);
void ToggleMultimapWaypoints(void);
void ToggleMultimapOverlays(void);

bool IsMultimapTerrain(void);
bool IsMultimapAirspace(void);
bool IsMultimapTopology(void);
bool IsMultimapWaypoints(void);
bool IsMultimapOverlays(void);

void EnableMultimapTerrain(void);
void EnableMultimapAirspace(void);
void EnableultimapTopology(void);
void EnableMultimapWaypoints(void);
void EnableMultimapOverlays(void);

void DisableMultimapTerrain(void);
void DisableMultimapAirspace(void);
void DisableMultimapTopology(void);
void DisableMultimapWaypoints(void);
void DisableMultimapOverlays(void);

unsigned short GetMultimap_Labels(void);
void SetMultimap_Labels(const unsigned short);
short Get_Current_Multimap_Type();
void Reset_Multimap_Flags(void);


#endif