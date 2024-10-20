- [x] Create BlueprintFunctionLibrary for accessing elements through tags and actors
- [x] Add element interface so that anything can define its element

- [x] Added two events in Character base for attack begin and attack end (for communication between animations and abilities)

- [x] Make A Projectile Attack ability base class (in C++)
- [x] Make A First Projectile Attack in blueprints
- [x] Make Player Quick Cast use projectile attack ability
- [ ] Ensure this attack kills the dummy
- [ ] determine if 
- [ ] fix projectile so that it only checks for overlaps if authority

- [ ] Make a second projectile attack ability in blueprints for enemy
- [ ] Add a field in wisp that allows specifying a testing attack target 
- [ ] Give this ability to wisp
- [ ] Make a Task that can call an ability
- [ ] Make a Decorator that can check if an ability is in cooldown
- [ ] Make a Decorator that can check if an ability can be used
- [ ] Make a Task that can retrieve a target trajectory given an attack target


- [ ] Create a larger testing arena containing 4 combat zones for testing 

- [ ] Create structure S_PT_WallAssets
	-  [ ] wall_assets
	- [ ] door_assets
	- [ ] window_assets
	- [ ] corner_assets
- [ ] Create structure S_PT_HouseSideSwitches
	- [ ] on_all
	- [ ] on_front
	- [ ] on_right
	- [ ] on_left
	- [ ] on_back
- [ ] PCG_ReadSideSwitches
	- [ ] input: 
		- [ ] switch
	- [ ] output: 
		- [ ] on_front, on_right, on_left, on_back
- [ ] PCG_Test_VillageWall_Single
	- [ ] parameters: 
		- [ ]  cnt=4, 
		- [ ] size=200,200,220,
		- [ ] seed=5125125, 
		- [ ] right_corner_only=true,
		- [ ] spawn_corners=false,
		- [ ] spawn_door=true,
		- [ ] door_index=3,
		- [ ] spawn_windows=true,
		- [ ] window_percent=1.0,
		- [ ] wall_assets, 
		- [ ] door_assets, 
		- [ ] window_assets, 
		- [ ] corner_assets,
		- [ ] remove_percentage=0.75
- [ ] PCG_VillageHouse_Walls
	- [ ] parameters:  
		- [ ] cnt=4, 
		- [ ] size=200,200,220,
		- [ ] seed=5125125, 
		- [ ] assets=S_PT_WallAssets
		- [ ] remove_percentages=1.0
		- [ ] per_side_remove_percentages = 0.75,0.75,0.75,0.75
		- [ ] doors : S_PT_HouseSideSwitches
		- [ ] windows : S_PT_HouseSideSwitches
		

- [x] PCG_VillageWall_Single
	- [x] parameters: 
		- [ ] cnt=4, 
		- [ ] size=200,200,220,
		- [ ] seed=5125125, 
		- [ ] right_corner_only=true,
		- [ ] spawn_corners=false,
		- [ ] spawn_door=true,
		- [ ] door_index=3,
		- [ ] spawn_windows=true,
		- [ ] window_percent=1.0,
		- [ ] wall_assets, 
		- [ ] door_assets, 
		- [ ] window_assets, 
		- [ ] corner_assets,
		- [ ] remove_percentage=0.75
	- [x] add inputs:
		- [x] copy target
	- [x] add outputs:
		- [x] wall points
- [x] PCG_CreateVillageWall
	- [x] add parameters: cnt, size, local_space, seed, right corner only = true
	- [x] add inputs
		- [x] spawn grid
		- [x] flag r/l side spawns: add attribute **side** (1, 0, -1)
		- [x] get distance to center: copy to attribute **center distance**
		- [x] create corners
	- [ ] add outputs: walls, corners
- [x] PCG_AddDoorToVillageWall
	- [x] add parameters: seed, count, door_index
	- [x] add inputs: wall points
	- [x] add outputs: wall, doors
- [x] PCG_AddWindowToVillageWall
	- [x] add parameters: seed, window percent, flip_on_negative=true
	- [x] add inputs: walls
	- [x] add outputs: walls, windows
- [x] PCG_InstanceAssetsForVillageWall
	- [ ] add parameters: wall, window, door, corner, spawn corners
	- [x] add inputs: walls, doors, windows, corners
	- [x] add outputs: set




# Spawner Bitmask
wall vs corner = 1 (0 = wall, 1 = corner)

wall type = bits 2,3
00 = none
01 = window
10 = door
11 = wall

walltype bits (0-3)
cornertype bits (4-5)

## Corners Bitmask
corner type is set? = 2  (0 = no, 1 = yes)
corner type = 3 (1 = out, 0 = inside)

allow windows
allow doors
