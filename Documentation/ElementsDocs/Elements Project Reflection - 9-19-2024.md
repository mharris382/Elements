
# please read the readme 

started reading the documentation project readme on my phone and realized how much more I would get out of it now that I've done a bit of work with GAS.  My first step tomorrow should be to go find the most critical parts of that readme (**Exec, GameplayEffects, MMM, tag events, gameplay events**) and create a note on them
# Attempt Multiplayer?
While attempting to support multiplayer is certainly an increase to scope, there are several reasons I feel that it should still be attempted.
1. I need to actually develop this very important skillset, and I have yet to actually move forward with a serious project that employs multiplayer/networking
2. This project would be an ideal case for learning multiplayer because with PvE gameplay and a small number of clients, there is less need to rely on prediction
3. GAS is designed to support multiplayer and solves a lot of problems with networking/multiplayer
## Debugging Multiplayer
I don't think this is quite urgent yet, as I am still very much getting a feel for GAS as well as AI, but once the gameplay starts taking shape, that would be the time to stop and ensure that multiplayer works correctly

```ad-note
title: Note: GAS Documentation 
the gas documentation readme page lists some common multiplayer mistakes/errors so I should definetly check there to see if I can isolate the issue.
```


# Cooldown Tags
- cooldown effects are supposed to have tags associated with them for replication


# Dash Ability Class
- the montage looks lame
- probably make a C++ base class that uses movement


# Projectile Ability Base Class

```ad-attention
title: Attention: Update to Targeting System
The current targeting system is awkard at best, though the central idea was still a good one.  However, the following changes should be made to improve the usability and optimization with GAS.
- use the targeting data structure provided by GAS instead of returning a projectile spawn transform (or in addition?)
- only expose one function for aiming, rather than multiple functions that are acting in a similar way (get target vs get ws transform)
```

## Projectile Base Class

```ad-important
title: Important: expose variables that the AI would need to make decisions
One thing I just learned from making an enemy that uses the a projectile attack ability is that they can use values from the default class object **before** choosing to use an ability or spawn a projectile.   However, they can only get values that are set as variables, they cannot get the settings on child components (as far as I can tell).  

Any values that would help an AI make decisions should therefore be exposed as variables on the actor class so the AI can retreive those values without spawning the actor.

The example here is spawning a projectile.  The AI needs to know the initial velocity and gravity scale of a projectile in order to properly calculate the trajectory that is needed to determine where to spawn the projectile
```

# Damage Exec Calculation
- This needs to be understood as a priority.  This class is fundamentally important to GAS as well as the critical gameplay logic.  Adding debugging lines that print everything going on inside this function is the next step.


# AI WTF
I need to rewatch that AI series that I did, because the AI I keep attempting to make for this project is pooping in the nuts.  

- I can potentially ask Steph to assist with this but I think she is very focused on figuring out the workflow for blender to Unreal.

# UI
I really want to do this, but I don't know if it is yet a priority.  It sounds really fun to make an interesting juicy UI, but gameplay and fully working GAS/AI should come first.  

# Melee Abilities
I think these can actually be moved up in the priority queue.  I think melee will work very nicely and is an Action RPG staple.     

## Quick Summon Melee
I should start with quick summon melee attacks, which would be basically a move where the character performs an animation that goes with a melee weapon, and the weapon manifests out of their element during the attack, then disappears afterwards


# Melee Only Abilities for enemies
I think melee only should be a thing for certain enemies.   This could pretty much reuse the same logic for the quick summon melee but all the time

##  Summon Perma Melee
this would be a special more complex ability that actually changes the character's ability set (potentially temporarily), giving access to a summoned melee weapon that could be used to attack without consuming mana; but also preventing core spells from being used. 

once summoned, it could be removed by using the same button that created it.



# Blocking
Not quite ready for this yet. 

For the future: I think it will play interestingly if blocking has a different interaction with melee vs ranged




