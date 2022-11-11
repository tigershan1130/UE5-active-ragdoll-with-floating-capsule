# Proof of concept UE5 active ragdoll :
This is a proof of concept of Active Ragdoll + Floating Capsule(rigdibody dynamics capsule controller)
There are many floating capsule and active ragdoll projects for Unity, however it is harder to implement those effect in UE.
This is a proof of concept having both working together.

Physical Animation Based Active Ragdoll:

Similar to UE's Physical Animation, except written from the animation node to read animation keyframe pose and adjust each bone accordingly.

Floating Capsule Information: https://www.youtube.com/watch?v=qdskE8PJy6Q&t=171s

Example: reaction effect fully driven by physics where hitting certain part of the body would drive phyiscal force moving root/hips.

Physical Reaction 1:

![alt text](https://github.com/tigershan1130/UE5-active-ragdoll-with-floating-capsule/blob/main/ScreenShots/Phyysics4.gif)

Physical Reaction 2:

![alt text](https://github.com/tigershan1130/UE5-active-ragdoll-with-floating-capsule/blob/main/ScreenShots/Physics1.gif)

Floating capsule going upstairs:

![alt text](https://github.com/tigershan1130/UE5-active-ragdoll-with-floating-capsule/blob/main/ScreenShots/Physics2.gif)


