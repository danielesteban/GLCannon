package com.gatunes.glcannon;
import org.libsdl.app.SDLActivity;
public class GLCannonActivity extends SDLActivity {
  protected String[] getLibraries() {
      return new String[] {
          "SDL2",
          "SDL2_image",
          "Bullet",
          "main"
      };
  }
}
