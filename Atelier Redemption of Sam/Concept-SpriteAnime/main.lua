io.stdout:setvbuf('no')
if arg[#arg] == "-debug" then require("mobdebug").start() end

-- Globales utiles
local largeurEcran
local hauteurEcran

-- Les listes
local liste_sprites = {}

-- Le héros !
local samSprite = {}

function CreeSprite(pNomImage, pNbImages, pX, pY)
  
  sprite = {}
  sprite.x = pX
  sprite.y = pY
  sprite.frame = 1
  sprite.vx = 0
  sprite.vy = 0
  sprite.flip = 1
  sprite.images = {}
  local imgNum
  for imgNum = 1,pNbImages do
    sprite.images[imgNum] = love.graphics.newImage("images/"..pNomImage..imgNum..".png")
  end   
  sprite.l = sprite.images[1]:getWidth()
  sprite.h = sprite.images[1]:getHeight()
  
  table.insert(liste_sprites, sprite)
  
  sprite.Anime = function()
    sprite.frame = samSprite.frame + 0.4
    if sprite.frame >= #sprite.images then
      sprite.frame = 1
    end
  end
  
  sprite.Deplace = function()
    -- Réduction de la vélocité (=friction)
    sprite.vx = sprite.vx * .9
    sprite.vy = sprite.vy * .9
    if math.abs(sprite.vx) < 0.01 then sprite.vx = 0 end
    if math.abs(sprite.vy) < 0.01 then sprite.vy = 0 end
    
    -- Application de la vélocité
    sprite.x = sprite.x + sprite.vx
    sprite.y = sprite.y + sprite.vy
  end
  
  return sprite
  
end

function DemarreJeu()
  -- Vide la liste des sprites
  liste_sprites = {}
  
  -- Crée Sam
  samSprite = CreeSprite("p1_walk",11,0,0)
  
  -- Centre Sam à l'écran
  samSprite.x = largeurEcran/2
  samSprite.y = hauteurEcran/2
end

function love.load()
  
  love.window.setTitle( "Redemption of Sam (c) 2015 David Mekersa from Gamecodeur.fr" )
  love.window.setMode(1024, 768, {fullscreen=false, vsync=true}) --, minwidth=1024, minheight=768})
  
  largeurEcran = love.graphics.getWidth()
  hauteurEcran = love.graphics.getHeight()
  
  DemarreJeu()
  
end

function love.update(dt)

  -- Stocke la position actuelle de Sam
  local ancienX = samSprite.x
  local ancienY = samSprite.y
  
  local n
  for n=1,#liste_sprites do
    local s = liste_sprites[n]
    s.Deplace()
    s.Anime()
  end
  
  -- Changement de frame d'animation pour Sam
  -- Si Sam n'avance pas, il ne bouge plus les jambes
  if math.abs(samSprite.vx) < 1 and math.abs(samSprite.vy) < 1 then
    samSprite.frame = 1
  end
  
  -- Détection des touches du clavier
  if love.keyboard.isDown("d") then
      samSprite.vx = samSprite.vx + 1
      samSprite.flip = 1
  end
  if love.keyboard.isDown("q") or love.keyboard.isDown("a") then
      samSprite.vx = samSprite.vx - 1
      samSprite.flip = -1
  end
  if love.keyboard.isDown("z") or love.keyboard.isDown("w") then
      samSprite.vy = samSprite.vy - 1
  end
  if love.keyboard.isDown("s") then
      samSprite.vy = samSprite.vy + 1
  end
end

function love.draw()
  -- On dessine les sprites
  local n
  for n=1,#liste_sprites do
    local s = liste_sprites[n]
    local frame = s.images[math.floor(s.frame)]
    love.graphics.draw(frame, s.x, s.y, 0, s.flip, 1, s.l/2, s.h-6)
  end
  
  -- Informations de debug
  love.graphics.print("Vitesse de sam x:"..samSprite.vx.." y:"..samSprite.vy)
end

function love.keypressed(key)
  if key == "space" or key == " " then
    DemarreJeu()
  end
end