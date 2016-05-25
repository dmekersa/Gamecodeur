io.stdout:setvbuf('no')
if arg[#arg] == "-debug" then require("mobdebug").start() end

-- Globales utiles
local largeurEcran
local hauteurEcran

-- Le héros !
local samSprite = {}
samSprite.images = {}
local imgNum
for imgNum = 1,11 do
  samSprite.images[imgNum] = love.graphics.newImage("images/p1_walk"..imgNum..".png")
end   
samSprite.frame = 1
samSprite.vx = 0
samSprite.vy = 0
samSprite.flip = 1
samSprite.h = samSprite.images[1]:getHeight()
samSprite.w = samSprite.images[1]:getWidth()

function DemarreJeu()
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

  -- Réduction de la vélocité (=friction)
  samSprite.vx = samSprite.vx * .9
  samSprite.vy = samSprite.vy * .9
  if math.abs(samSprite.vx) < 0.01 then samSprite.vx = 0 end
  if math.abs(samSprite.vy) < 0.01 then samSprite.vy = 0 end
  
  -- Application de la vélocité à Sam
  samSprite.x = samSprite.x + samSprite.vx
  samSprite.y = samSprite.y + samSprite.vy
  
  -- Changement de frame d'animation pour Sam
  -- Si Sam n'avance pas, il ne bouge plus les jambes
  if math.abs(samSprite.vx) < 1 and math.abs(samSprite.vy) < 1 then
    samSprite.frame = 1
  else
    samSprite.frame = samSprite.frame + 24*dt
    if samSprite.frame >= 12 then
      samSprite.frame = 1
    end
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
  -- On dessine Sam
  local samFrame = samSprite.images[math.floor(samSprite.frame)]
  love.graphics.draw(samFrame, samSprite.x, samSprite.y, 0, samSprite.flip, 1, samSprite.w/2, samSprite.h-6)
  
  -- Informations de debug
  love.graphics.print("Vitesse de sam x:"..samSprite.vx.." y:"..samSprite.vy)
end

function love.keypressed(key)
  if key == "space" or key == " " then
    DemarreJeu()
  end
end