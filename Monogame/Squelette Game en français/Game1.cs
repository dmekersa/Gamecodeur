using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;

namespace Game1
{
    // Classe principale du jeu
    public class Game1 : Game
    {
        GraphicsDeviceManager graphics;
        SpriteBatch spriteBatch;

        public Game1()
        {
            graphics = new GraphicsDeviceManager(this);
            Content.RootDirectory = "Content";
        }

        // Fonction appelée une fois pour initialiser le jeu
        protected override void Initialize()
        {
            // TODO: Ajoutez ici votre code d'initialisation

            base.Initialize();
        }

        // Fonction appelée une seule fois pour charger le contenu du jeu
        protected override void LoadContent()
        {
            // Crée un nouveau SpriteBatch, qui sera utilisé pour afficher des images (textures)
            spriteBatch = new SpriteBatch(GraphicsDevice);

            // TODO: Ajoutez ici votre code qui chargera le contenu du jeu
        }

        // Fonction appelée une fois pour décharger le contenu du jeu (hors ContentManager)
        protected override void UnloadContent()
        {
            
        }

        // Fonction appelée 60x par seconde pour mettre à jour l'état du jeu
        // Reçoit "gametime" qui contient le temps écoulé depuis le dernier update
        protected override void Update(GameTime gameTime)
        {
            if (GamePad.GetState(PlayerIndex.One).Buttons.Back == ButtonState.Pressed || Keyboard.GetState().IsKeyDown(Keys.Escape))
                Exit();

            // TODO: Ajoutez le code de mise à jour ici

            base.Update(gameTime);
        }

        // Fonction appelée aussi souvent que possible (jusqu'à 60x par seconde) pour afficher le jeu
        // Reçoit "gametime" qui contient le temps écoulé depuis le dernier update
        protected override void Draw(GameTime gameTime)
        {
            GraphicsDevice.Clear(Color.CornflowerBlue);

            // TODO: Ajouter le code d'affichage ici

            base.Draw(gameTime);
        }
    }
}
