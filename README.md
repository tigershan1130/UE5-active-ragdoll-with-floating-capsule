# Proof of concept UE5 active ragdoll :
This is a proof of concept of Active Ragdoll + Floating Capsule(rigdibody dynamics capsule controller)
There are many floating capsule and active ragdoll projects for Unity, however it is harder to implement those effect in UE.
This is a proof of concept having both working together.

Physical Animation Based Active Ragdoll:

Similar to UE's Physical Animation, except written from the animation node to read animation keyframe pose and adjust each bone accordingly.

Floating Capsule Information: https://www.youtube.com/watch?v=qdskE8PJy6Q&t=171s

Example: reaction effect fully driven by physics where hitting certain part of the body would drive phyiscal force moving root/hips.

Reaction 1:
![alt text]()

Reaction 2:
![alt text]()

Reaction 3:
![alt text]()
