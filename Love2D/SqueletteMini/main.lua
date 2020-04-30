-- Cette ligne permet d'afficher des traces dans la console pendant l'éxécution
io.stdout:setvbuf('no')

-- Empèche Love de filtrer les contours des images quand elles sont redimentionnées
-- Indispensable pour du pixel art
love.graphics.setDefaultFilter("nearest")

-- Cette ligne permet de déboguer pas à pas dans ZeroBraneStudio
if arg[#arg] == "-debug" then require("mobdebug").start() end

function love.load()
  
  largeur_ecran = love.graphics.getWidth()
  hauteur_ecran = love.graphics.getHeight()
  
end

function love.update(dt)

end

function love.draw()
    
end

function love.keypressed(key)
  
  print(key)
  
end
  