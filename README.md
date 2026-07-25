Breakout

You start by designing your own spaceship in a block editor, and then that ship
becomes the paddle you play Breakout with.

Ship Editor:  
    - Drag & drop blocks from the side panel onto the grid.  
    - Right-click to remove a block you already placed.  
    - Before you can start the game, a few constraints have to be met:  
        -> At least 1 block, but no more than 10.  
        -> All blocks have to be connected into a single ship.  
        -> Nothing can go below an engine or above a cannon.  
    - The start button only turns green once everything checks out.  

Gameplay:  
    - You get 3 lives, and lose one every time the ball slips past the paddle.  
    - Press SPACE to launch the ball at a 45 degree angle.
    - Collisions use circle-vs-AABB for the ball against bricks, paddle and walls.  
    - The angle the ball bounces off the paddle depends on where it hits.  
    - Bricks have health - some take up to 5 hits to break, and their color  
    changes depending on how much health is left.  
    - Bricks shrink down with a little animation right before they disappear.  
    - Hitting a brick or the paddle throws out a small particle burst.  
    - There's a screen shake effect on impact.  
    - The ball gets faster the higher your score goes.  
    - When it's game over you can either restart or quit from that screen.  
