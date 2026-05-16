# **AI DIGIT RECOGNIZER**
#### **Video Demo:**  https://www.youtube.com/watch?v=-n1uytGfoBQ
#### **Description:** This is an application made in Godot 4.6 and C++ through GDExtensions, it consists of a canvas with some buttons in which you can draw. As you draw, a Neural Network written in C++ will try to predict what digit is currently drawn.

## **GODOT UI**
![A screenshot of the app with a two drawn](screenshots/screenshot1.png)

This is the thing you'll see, everything related to user interface and image pre-procesing happens in the `project/` folder
the Godot scenes are in `project/scenes/`, the GDScript source code is in `project/src/` and all the images, fonts, and Godot themes are in `project/assets`.

> [!note]
> The `project/bin/` folder stores the `.gdextension` and `.dll`s that make C++ work in Godot.

### **Scenes**

- **`main.tscn`**: this is, well, the main scene. It just has an instance of other scenes to put it all together, it also has a background ColorRect which has a subtle animation with colors changing between different shades of blue and purple.

- **`paint_widget.tscn`**: although not the main scene, it is the most important part. This scene handles drawing, the compression and pre-process of the drawn image before passing it to the neural network.

- **`controller_button.tscn`**: this is a customized button which has a liquid glass style and smooth animations.

- **`paint_widget_controller.tscn`**: this scene just mixes four controller buttons, the paint widget and decorates them with a liquid-glass-styled panel.

- **`output_neurons.tscn`**: this consists of two iquid-glass-styled panels and two labels with a translucent style.
The panels imitate a "neuron", a line connected to a circle which can be more or less activated, that is controlled through a shader which tints it white depending on the value. One of the labels just indicates what number the neuron corresponds to, the smaller one indicates the confidence of that number being the one you drew.

- **`output_vector.tscn`**: this is just ten output neurons in a column, each with its corresponding digit, which indicates the full output vector of the neural network.

- **`big_digit_value_pair.tscn`**: this is a bigger version of that digit-confidence pair each output neuron had, used to display the digit the network guessed.

### **GDScript Code**

> [!WARNING]
> Most of the shaders are generated with AI (I dont't know how to write them yet), as well as some parts of the GDScript code.

> [!NOTE]
> Each script is found inside a folder with the same name as the scene it is associated with, except `global.gd`, `main.gdshader`, and `logo.gdshader`.

- **`paint_widget.gd`**: among its properties we have:
    - **`image`, `compressed_image`, `centered_image`, `brush_overlay_image`**: they are images which store the drawn image, the downsized image, a centered version of the image and an image which just shows the brush, respectively.

        > [!NOTE]
        > `brush_overlay_image` is an `Image` instead of being drawn in the Godot `_draw()` function because then I can apply a shader to round corners.
    
    - **`_image_dirty`, `_overlay_dirty`, `_centered_image_dirty`**: these are some boolean flags to update the images just when necessary. So if nothing new is drawn, it won't be updated.
- **`controller_button.gd`**:
- **`paint_widget_controller.gd`**:
- **`output_neurons.gd`**:
- **`output_vector.gd`**:
- **`big_digit_value_pair.gd`**:

