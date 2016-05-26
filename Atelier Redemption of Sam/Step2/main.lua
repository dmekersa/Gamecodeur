io.stdout:setvbuf('no')
if arg[#arg] == "-debug" then require("mobdebug").start() end

local spriteManager = require("spritemanager")

-- Globales utiles
local largeurEcran
local hauteurEcran

-- Constantes
local LARGEURTILE = 64
local HAUTEURTILE = 64

-- Le donjon
local donjon = require("donjon")

local samSprite = {}
local salleFond = {}

function DemarreJeu()
  
  samSprite = spriteManager.CreeSprite("p1_walk",11,0,0)
  
  samSprite.x = largeurEcran/2
  samSprite.y = hauteurEcran/2
  
end

function love.load()
  
  love.window.setTitle( "Redemption of Sam (c) 2015 David Mekersa from Gamecodeur.fr" )
  love.window.setMode(1024, 768, {fullscreen=false, vsync=true}) --, minwidth=1024, minheight=768})
  
  largeurEcran = love.graphics.getWidth()
  hauteurEcran = love.graphics.getHeight()
  
  salleFond = love.graphics.newImage("images/salle.png")
  
  DemarreJeu()
  
end

function love.update(dt)
  
  spriteManager.update(dt)
  
  if math.abs(samSprite.vx) < 1 and math.abs(samSprite.vy) < 1 then
    samSprite.frame = 1
  end
  
  if love.keyboard.isDown("d") then
    samSprite.vx = samSprite.vx + 1
    samSprite.flip = 1
  end
  if love.keyboard.isDown("q") then
    samSprite.vx = samSprite.vx - 1
    samSprite.flip = -1
  end
  if love.keyboard.isDown("z") then
    samSprite.vy = samSprite.vy - 1
  end
  if love.keyboard.isDown("s") then
    samSprite.vy = samSprite.vy + 1
  end
  
end

function love.draw()
  
  love.graphics.draw(salleFond,0,0)
  
  spriteManager.draw()
    
end

function love.keypressed(key)
  if key == "space" or key == " " then
    DemarreJeu()
  end
end