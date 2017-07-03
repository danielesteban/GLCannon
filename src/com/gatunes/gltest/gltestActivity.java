package com.gatunes.gltest;
import org.libsdl.app.SDLActivity;
public class gltestActivity extends SDLActivity {
  protected String[] getLibraries() {
      return new String[] {
          "SDL2",
          "SDL2_image",
          "main"
      };
  }
}
