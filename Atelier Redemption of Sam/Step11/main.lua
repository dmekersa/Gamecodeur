io.stdout:setvbuf('no')
if arg[#arg] == "-debug" then require("mobdebug").start() end

-- Constantes
LARGEURTILE = 64
HAUTEURTILE = 64

local spriteManager = require("spritemanager")
local bulletManager = require("bulletmanager")
local particleManager = require("particlemanager")

-- Globales utiles
local largeurEcran
local hauteurEcran

-- Le game state
local gameState = require("gamestate")

-- Le donjon
local donjon = require("donjon")

local samSprite = {}
local salleFond = {}

local salleCourante = {}
salleCourante.portes = {}

function TestePorte(pX,pY)
  local n
  
  for n=1,#salleCourante.portes do
    local c = salleCourante.portes[n]

    if pX > c.x and pX < c.x + c.largeur and
       pY > c.y and pY < c.y + c.hauteur then
       return c.type
    end

  end

  return ""
end

function CreePorte(pType, pX, pY, pLargeur, pHauteur)
  
  print("Ajoute une porte de type",pType)
  
  local newPorte = {}
  
  newPorte.x = pX
  newPorte.y = pY
  newPorte.largeur = pLargeur
  newPorte.hauteur = pHauteur
  
  newPorte.type = pType
  
  return newPorte
end

function ChargeSalle(pSalle)
  
  bulletManager.Reset()
  
  salleCourante.portes = {}
  
  if pSalle.porteHaut == true then
    local porte = CreePorte("porteHaut",(largeurEcran/2)-LARGEURTILE,0+5,LARGEURTILE*2,HAUTEURTILE*2)
    table.insert(salleCourante.portes, porte)
  end
  if pSalle.porteDroite == true then
    local porte = CreePorte("porteDroite",largeurEcran-(LARGEURTILE*2)-5,(hauteurEcran/2)-HAUTEURTILE,LARGEURTILE*2,HAUTEURTILE*2)
    table.insert(salleCourante.portes, porte)
  end
  if pSalle.porteBas == true then
    local porte = CreePorte("porteBas",(largeurEcran/2)-LARGEURTILE,hauteurEcran-(HAUTEURTILE*2)-5,LARGEURTILE*2,HAUTEURTILE*2)
    table.insert(salleCourante.portes, porte)
  end
  if pSalle.porteGauche == true then
    local porte = CreePorte("porteGauche",0+5,(hauteurEcran/2)-HAUTEURTILE,LARGEURTILE*2,HAUTEURTILE*2)
    table.insert(salleCourante.portes, porte)
  end
  
  salleCourante.salle = pSalle
  
end

function DemarreJeu()
  
  spriteManager.Reset()
  
  samSprite = spriteManager.CreeSprite("p1_walk",11,0,0)
  samSprite.offsetY = (samSprite.h/2) - 6
  
  samSprite.x = largeurEcran/2
  samSprite.y = hauteurEcran/2
  
  donjon.GenereDonjon()
  
  ChargeSalle(donjon.salleDepart)
  
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
  
  particleManager.update(dt)
  
  local ancienX = samSprite.x
  local ancienY = samSprite.y
  
  spriteManager.update(dt)
  bulletManager.update(dt)
  
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
  
  -- collision avec les portes
  local nouvelleSalle = nil
  local typePorte = TestePorte(samSprite.x,samSprite.y)
  if typePorte ~= "" then
    print("Collision avec",typePorte)
    local newX, newY
    newX = samSprite.x
    newY = samSprite.y
    if typePorte == "porteHaut" then
      nouvelleSalle = donjon.map[salleCourante.salle.ligne-1][salleCourante.salle.colonne]
      newX = samSprite.x
      newY = hauteurEcran-(HAUTEURTILE*2)-(samSprite.h/2)-10
    elseif typePorte == "porteDroite" then
      nouvelleSalle = donjon.map[salleCourante.salle.ligne][salleCourante.salle.colonne+1]
      newX = 0+(LARGEURTILE*2)+10
      newY = samSprite.y
    elseif typePorte == "porteBas" then
      nouvelleSalle = donjon.map[salleCourante.salle.ligne+1][salleCourante.salle.colonne]
      newX = samSprite.x
      newY = 0+(HAUTEURTILE*2)+(samSprite.h/2)+10
    elseif typePorte == "porteGauche" then
      nouvelleSalle = donjon.map[salleCourante.salle.ligne][salleCourante.salle.colonne-1]
      newX = largeurEcran-(LARGEURTILE*2)-10
      newY = samSprite.y
    end
    if nouvelleSalle ~= nil then
      ChargeSalle(nouvelleSalle)
      samSprite.x = newX
      samSprite.y = newY
    end
  end
  
  -- collision avec les murs
  if nouvelleSalle == nil then
    local nColonneCollision
    local nLigneCollision
    local bHorizontal = false
    local bVertical = false
  
    -- Dabord test horizontal
    nColonneCollision = math.floor((samSprite.x / LARGEURTILE) + 1)
    nLigneCollision = math.floor((ancienY / HAUTEURTILE) + 1)
    if gameState.mapMurs[nLigneCollision][nColonneCollision] > 0 then
      bHorizontal = true
    end
    
    -- Ensuite test verticalement
    nColonneCollision = math.floor((ancienX / LARGEURTILE) + 1)
    nLigneCollision = math.floor((samSprite.y / HAUTEURTILE) + 1)
    if gameState.mapMurs[nLigneCollision][nColonneCollision] > 0 then
      bVertical = true
    end
    
    if bHorizontal then
      samSprite.x = ancienX
      samSprite.vx = 0
    end
    
    if bVertical then
      samSprite.y = ancienY
      samSprite.vy = 0
    end
    
  end
  
end

function love.draw()
  
  love.graphics.draw(salleFond,0,0)
  
  local nPorte
  for nPorte = 1,#salleCourante.portes do
    local p = salleCourante.portes[nPorte]
    love.graphics.rectangle("line",p.x,p.y,p.largeur,p.hauteur)
  end
  
  donjon.DessineMapDonjon(salleCourante.salle)
    
  spriteManager.draw()
  bulletManager.draw()
  particleManager.draw()
  
  love.graphics.circle("fill",samSprite.x,samSprite.y,5)
      
end

function love.keypressed(key)
  if key == "space" or key == " " then
    DemarreJeu()
  end
end

function love.mousepressed(x, y, button, istouch)
  if button == 1 then -- the primary button
    local dx = x - samSprite.x
    local dy = y - samSprite.y
    
    if math.abs(dx) > math.abs(dy) then
      print("Tir horizontal")
      if dx < 0 then
        print("gauche")
        bulletManager.CreeBullet("sam",samSprite.x,samSprite.y-samSprite.h/2,-10,0.6)
      else
        print("droite")
        bulletManager.CreeBullet("sam",samSprite.x,samSprite.y-samSprite.h/2,10,0.6)
      end
    else
      print("Tir vertical")
      if dy < 0 then
        print("haut")
        bulletManager.CreeBullet("sam",samSprite.x,samSprite.y-samSprite.h/2,0,-10)
      else
        print("bas")
        bulletManager.CreeBullet("sam",samSprite.x,samSprite.y-samSprite.h/2,0,10)
      end
    end
    
  end
end