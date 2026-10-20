# **Network Tank Arena**


## Problem 

Combat networking 



## Baseline 

One tank per client can be moved around using WASD and aim in the direction of the mouse, the cannon rotating toward mouse location 



## Plan 

I will implement server-authoritative projectile combat where clients need to agree on health, hits and death, while invalid firing requests are rejected. 

- Projectile hits generate push-back, applied on the server, replicated down 

- Death makes tanks non moveable and changes the appearance of the tank (state change) 

- Projectiles can destroy parts of the level (state change, nice to have) 


### Simple projectile shot flow:  

- Client requests to shoot through RPC ->  

- Server validates and spawns projectile -> 

- Spawned actor-projectile that replicates is at the same time spawned on all connected clients -> 

- Projectile movement is handled serverside and replicated down to client ->  

- Collisions are handled serverside and replicated down ->  

- Potential changes from the projectile collision are applied serverside and then replicated to client 



## Evidence 

I will test mostly through repeatable tests and make sure that: 

- Client actions can not violate authoritative truth, so shots cannot be fired if player has no ammo or if cannon is on cooldown 

- Death is synchronized to all clients and hits are registered correctly for server and client. 

- Make sure that, under network limitations, important packages still reach their destinations, while less important ones can be dropped, so combat should adhere to the same rules and timing regardless of network condition. 


----------------------


### Starter readme below:

Student starter for the Network Programming lab. Requires **Unreal Engine 5.7.4**
and the Windows C++ toolchain required by Unreal. Open `NetworkTankArena.uproject`
with UE 5.7.4 and build the project if prompted. The startup level is `TestMap`.

W/S move forward/backward, A/D turn the chassis, and the mouse aims the turret
independently. Weapons are not included in this lab baseline.

To run two players locally:

1. Open `TestMap` and select **Play → Advanced Settings**.
2. Set **Number of Players = 2** and **Net Mode = Play As Listen Server**.
3. Use **New Editor Window (PIE)** and enable **Run Under One Process**.
4. Select **Default Player Start**, then Play. Focus each player window in turn
   to control that player's tank. Shift+F1 releases the mouse from PIE.
5. Each window should see both tanks move and aim. Stop PIE and repeat.

Movement is server-authoritative without prediction; the client waits for server
updates, so some input delay and stepped motion are expected.

For a packaged **Development** build, start two copies. In the first copy, open
the console with tilde and enter `HostLocalhost`. In the second, enter
`JoinLocalhost`. This uses `127.0.0.1:7777`; start the host before joining.
These commands start from standalone play and require `TestMap` in the package.
No account or online service is required.
