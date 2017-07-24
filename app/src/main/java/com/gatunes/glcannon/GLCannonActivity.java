package com.gatunes.glcannon;
import org.libsdl.app.SDLActivity;
import android.content.Intent;
import android.net.Uri;
public class GLCannonActivity extends SDLActivity {
  protected String[] getLibraries() {
      return new String[] {
          "SDL2",
          "SDL2_image",
          "SDL2_mixer",
          "Bullet",
          "main"
      };
  }
  public void openGitHub() {
    Intent browserIntent = new Intent(
      Intent.ACTION_VIEW,
      Uri.parse("https://github.com/danielesteban/GLCannon")
    );
    startActivity(browserIntent);
  }
}
