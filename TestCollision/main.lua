-- Cette ligne permet d'afficher des traces dans la console pendant l'éxécution
io.stdout:setvbuf('no')

-- Empèche Love de filtrer les contours des images quand elles sont redimentionnées
-- Indispensable pour du pixel art
love.graphics.setDefaultFilter("nearest")

-- Cette ligne permet de déboguer pas à pas dans ZeroBraneStudio
if arg[#arg] == "-debug" then require("mobdebug").start() end

local sprite1 = {}
local sprite2 = {}
local liste_sprites = {}

function collide(a1, a2)
 if (a1==a2) then return false end
 local dx = a1.x - a2.x
 local dy = a1.y - a2.y
 if (math.abs(dx) < a1.image:getWidth()+a2.image:getWidth()) then
  if (math.abs(dy) < a1.image:getHeight()+a2.image:getHeight()) then
   return true
  end
 end
 return false
end

function CreeSprite(pNomImage, pX, pY)
  
  sprite = {}
  sprite.x = pX
  sprite.y = pY
  sprite.supprime = false
  sprite.image = love.graphics.newImage("images/"..pNomImage..".png")
  sprite.l = sprite.image:getWidth()
  sprite.h = sprite.image:getHeight()
  
  table.insert(liste_sprites, sprite)
  
  return sprite
  
end

function love.load()
  
  love.window.setMode(1024,768)
  love.window.setTitle("Test collision")
  
  largeur = love.graphics.getWidth()
  hauteur = love.graphics.getHeight()
  
  sprite1 = CreeSprite("heros", largeur/2, hauteur/2)
  sprite1.x = largeur/2
  sprite1.y = largeur/2 
    
  sprite2 = CreeSprite("enemy1", largeur/2, hauteur/2)
  sprite2.x = largeur/2
  sprite2.y = largeur/2 

end

function love.update(dt)
  
  if love.keyboard.isDown("right") then
    sprite1.x = sprite1.x + 4
  end

  if love.keyboard.isDown("left") then
    sprite1.x = sprite1.x - 4
  end
  
  if love.keyboard.isDown("up") then
    sprite1.y = sprite1.y - 4
  end

  if love.keyboard.isDown("down") then
    sprite1.y = sprite1.y + 4
  end

end

function love.draw()
  
  local n
  for n=1,#liste_sprites do
    local s = liste_sprites[n]
    love.graphics.draw(s.image, s.x, s.y, 0, 2, 2, s.l/2, s.h/2)
  end
  
  if collide(sprite1,sprite2) then
    love.graphics.print("Collision")
  end
   
end

function love.keypressed(key)
  
end
  