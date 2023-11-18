# CSE 167 Homework

## HW3: 3D OpenGL Rendering

### Renderings

The scene that I designed is in [balboa_public\scenes\hw3\scythe.json](balboa_public\scenes\hw3\scythe.json), with the image of the output in [balboa_public\outputs\scythe.png](balboa_public\outputs\scythe.png)

I implemented the 3.3 bonus where I use mouse input, as shown in the recordings.



---

## HW2: 3D Graphics

### Renderings

I have an extremely small bug that stopped my entire proccess for this project, were in 2.2 some of the faces that should be showing arn't. I was able to finish every thing I can:

- 2.1: Completed normaly.
- 2.2: Scene 1 is good, scenes 2 and 3 are a bit off.
- 2.3: The coloring logic is completly correct, but since I couldn't find the bug, the rendering issue persisted.
- 2.4: The parse_transformation function is completed, but again, 2.2 rendering issue persisted, don't have completed renderings.
- 2.5: Couldn't test my own scene with 2.2 rendering bug.

---


## HW1: 2D Graphics

### Scenes

 - For 1.7, the scene I created is called starry_sea.json in [./balboa_public/scenes/hw1/starry_sea.json](balboa_public/scenes/hw1/starry_sea.json)
 - For 1.8, I made a scene that would use lines, quadratic, and cubic besier lines called besier_lines.json in [balboa_public/scenes/hw1/besier_lines.json](balboa_public/scenes/hw1/besier_lines.json)

### Renderings

 - My rendering for starry_sea.json is called hw_1_7_starry_sea.png in [balboa_public/outputs/hw_1_7_starry_sea.png](balboa_public/outputs/hw_1_7_starry_sea.png)
 - All my png renderings for all hw1 parts ar in balboa_public/outputs

### Bonuses

I have done all the bonuses

 - For the 1.2 bonus, I put both the original 1.2 code and the bonus code in the same 1.2 function, since it wouldn't affect the outcome.
 - For the 1.4 bonus, the animation is called hw_1_4_bonus_colorblind_test.mp4 in the outputs directory [balboa_public/outputs/hw_1_4_bonus_colorblind_test.mp4](balboa_public/outputs/hw_1_4_bonus_colorblind_test.mp4).
 - For the 1.8 bonus, 
   - I made a new function hw_1_8 in hw1.cpp while implementing what I think is the implementation for making lines, quadratic, and cubic besier curves.
   - I also defined the function in main.cpp and hw1.h.
   - I made a new scene to test the function consisting of all three line types in [balboa_public/scenes/hw1/besier_lines.json](balboa_public/scenes/hw1/besier_lines.json).
   - Lastly, I made a new struct in [balboa_public/src/hw1_scenes.h](balboa_public/src/hw1_scenes.h) for a line, a quadratic, and a cubic besier curve to be used in my function.