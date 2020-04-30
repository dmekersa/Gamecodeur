io.stdout:setvbuf('no')
if arg[#arg] == "-debug" then require("mobdebug").start() end

-- Empèche Love de filtrer les contours des images quand elles sont redimentionnées
-- Indispensable pour du pixel art
love.graphics.setDefaultFilter("nearest")

-- Constantes
LARGEURTILE = 16
HAUTEURTILE = 16

require("util")

local spriteManager = require("spritemanager")
local bulletManager = require("bulletmanager")
local mobManager = require("mobmanager")
local particleManager = require("particlemanager")

-- Globales utiles
local largeurEcran
local hauteurEcran
local hauteurSalle = HAUTEURTILE*12

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
  mobManager.Reset()
  
  salleCourante.portes = {}
    
  if pSalle.porteHaut == true then
    local porte = CreePorte("porteHaut",(largeurEcran/2)-LARGEURTILE,(HAUTEURTILE*3),LARGEURTILE*2,HAUTEURTILE*2)
    if donjon.map[pSalle.ligne-1][pSalle.colonne] == donjon.salleBoss then
      porte.boss = true
    end
    table.insert(salleCourante.portes, porte)
  end
  if pSalle.porteDroite == true then
    local porte = CreePorte("porteDroite",largeurEcran-(LARGEURTILE*2),(hauteurSalle/2)+(HAUTEURTILE*3)-HAUTEURTILE,LARGEURTILE*2,HAUTEURTILE*2)
    if donjon.map[pSalle.ligne][pSalle.colonne+1] == donjon.salleBoss then
      porte.boss = true
    end
    table.insert(salleCourante.portes, porte)
  end
  if pSalle.porteBas == true then
    local porte = CreePorte("porteBas",(largeurEcran/2)-LARGEURTILE,hauteurEcran-(HAUTEURTILE*2),LARGEURTILE*2,HAUTEURTILE*2)
    if donjon.map[pSalle.ligne+1][pSalle.colonne] == donjon.salleBoss then
      porte.boss = true
    end
    table.insert(salleCourante.portes, porte)
  end
  if pSalle.porteGauche == true then
    local porte = CreePorte("porteGauche",0,(hauteurSalle/2)+(HAUTEURTILE*3)-HAUTEURTILE,LARGEURTILE*2,HAUTEURTILE*2)
    if donjon.map[pSalle.ligne][pSalle.colonne-1] == donjon.salleBoss then
      porte.boss = true
    end
    table.insert(salleCourante.portes, porte)
  end
  
  -- Spawn des monstres dans la salle
  if pSalle.mobPattern == 0 then
    -- Rien, la salle est vide
  elseif pSalle.mobPattern == 1 then
    -- Les mouches
    mobManager.CreeMob(1,(LARGEURTILE*2)+10,(HAUTEURTILE*5)+10,0,0)
    mobManager.CreeMob(1,largeurEcran-(LARGEURTILE*2)-10,(HAUTEURTILE*5)+10,0,0)
    mobManager.CreeMob(1,(LARGEURTILE*2)+10,hauteurEcran-(HAUTEURTILE*2)-10,0,0)
    mobManager.CreeMob(1,largeurEcran-(LARGEURTILE*2)-10,hauteurEcran-(HAUTEURTILE*2)-10,0,0)
  elseif pSalle.mobPattern == 2 then
    -- Les rats
  elseif pSalle.mobPattern == 3 then
    -- Défi : gérer un nouveau type de monstres
  end
  
  salleCourante.salle = pSalle
  
end

function DemarreJeu()
  
  spriteManager.Reset()
  
  samSprite = spriteManager.CreeSprite("sam_",8,0,0)
  samSprite.offsetY = (samSprite.h/2) - 3
  
  samSprite.x = largeurEcran/2
  samSprite.y = (HAUTEURTILE*3)+(hauteurSalle/2)
  
  samTete = spriteManager.CreeSprite("sam_tete_",4,0,0)
  samTete.x = samSprite.x
  samTete.y = samSprite.y
  samTete.offsetY = 15
  
  --samTeteCote
  
  -- =================== GENERER LE DONJON =======================
  donjon.GenereDonjon()
  
  local salleLaPlusLoin = donjon.salleLaPlusLoin()
  while donjon.nombreDePortes(salleLaPlusLoin) > 1 do
    print("********** Donjon invalide, on génère un nouveau")
    donjon.GenereDonjon()
    salleLaPlusLoin = donjon.salleLaPlusLoin()
  end
  donjon.salleBoss = salleLaPlusLoin
  donjon.salleBoss.mobPattern = 0
  
  -- ================== REPARTIR LES MONSTRES ===================
  local nbSalle = #donjon.listeSalles
  -- Moins la salle du boss
  nbSalle = nbSalle - 1
  -- Moins la salle de départ
  nbSalle = nbSalle - 1
  
  -- Choisir la salle de la clé
  local nSalleCle = math.random(1,#donjon.listeSalles)
  while donjon.listeSalles[nSalleCle] == donjon.salleDepart or donjon.listeSalles[nSalleCle] == donjon.salleBoss do
    nSalleCle = math.random(1,#donjon.listeSalles)
  end
  donjon.listeSalles[nSalleCle].dropCle = true
  donjon.listeSalles[nSalleCle].mobPattern = 3
  
  -- Répartir les patterns sur les salles restantes
  local listePattern = {}
  listePattern[0] = math.floor(nbSalle/3)
  listePattern[1] = math.floor(nbSalle/3)
  listePattern[2] = math.floor(nbSalle/3)
  
  local nPattern
  -- Pour chaque pattern
  for nPattern=0,#listePattern do
    local nSalle
    -- Pour chaque salle du pattern
    for nSalle=1,listePattern[nPattern] do
      local n = donjon.retourneSallePourMob()
      donjon.listeSalles[n].mobPattern = nPattern
    end
  end
  
  -- ================== CHARGER LA SALLE =========================
  ChargeSalle(donjon.salleDepart)
  
end

function love.load()
  
  love.window.setTitle( "Redemption of Sam (c) 2015 David Mekersa from Gamecodeur.fr" )
  love.window.setMode(320*3, 240*3, {fullscreen=false, vsync=true}) --, minwidth=1024, minheight=768})
  
  largeurEcran = love.graphics.getWidth()/3
  hauteurEcran = love.graphics.getHeight()/3
  
  salleFond = love.graphics.newImage("images/room.png")
  porteOuverte = love.graphics.newImage("images/door_open.png")
  porteFermee = love.graphics.newImage("images/door_closed.png")
  porteBoss = love.graphics.newImage("images/door_boss.png")
  
  DemarreJeu()
  
end

function love.update(dt)
  
  particleManager.update(dt)
  
  local ancienX = samSprite.x
  local ancienY = samSprite.y
  
  spriteManager.update(dt)
  bulletManager.update(dt)
  mobManager.update(dt)
  
  if math.abs(samSprite.vx) < 1 and math.abs(samSprite.vy) < 1 then
    samSprite.frame = 4
  end
  
  if love.keyboard.isDown("d") then
    samSprite.vx = samSprite.vx + 20*dt
    samSprite.flip = 1
  end
  if love.keyboard.isDown("q") then
    samSprite.vx = samSprite.vx - 20*dt
    samSprite.flip = -1
  end
  if love.keyboard.isDown("z") then
    samSprite.vy = samSprite.vy - 20*dt
  end
  if love.keyboard.isDown("s") then
    samSprite.vy = samSprite.vy + 20*dt
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
      newY = (3*HAUTEURTILE)+(hauteurSalle)-(2*HAUTEURTILE)
    elseif typePorte == "porteDroite" then
      nouvelleSalle = donjon.map[salleCourante.salle.ligne][salleCourante.salle.colonne+1]
      newX = 0+(LARGEURTILE*2)+10
      newY = samSprite.y
    elseif typePorte == "porteBas" then
      nouvelleSalle = donjon.map[salleCourante.salle.ligne+1][salleCourante.salle.colonne]
      newX = samSprite.x
      newY = (3*HAUTEURTILE)+(2*HAUTEURTILE)
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
  
  samTete.x = samSprite.x
  samTete.y = samSprite.y
  if math.abs(samSprite.vx) > math.abs(samSprite.vy) then
    samTete.frame = 2
    if samSprite.vx < 0 then
      samTete.flip = -1
    else
      samTete.flip = 1
    end
  else
    samTete.frame = 1
  end
  if math.abs(samSprite.vx) < 1 and math.abs(samSprite.vy) < 1 then
    samTete.frame = 1
  end
  
end

function love.draw()
  
  love.graphics.scale(3,3)
  
  love.graphics.draw(salleFond,0,3*HAUTEURTILE)
  
  local nPorte
  for nPorte = 1,#salleCourante.portes do
    local p = salleCourante.portes[nPorte]
    --love.graphics.rectangle("line",p.x,p.y,p.largeur,p.hauteur)
    local rotation = 0
    if p.type == "porteGauche" then
      rotation = 0-((math.pi*2)/4)
    elseif p.type == "porteDroite" then
      rotation = ((math.pi*2)/4)
    elseif p.type == "porteBas" then
      rotation = ((math.pi*2)/4)*2
    end
    love.graphics.draw(porteOuverte,p.x+16,p.y+16,rotation,1,1,16,16)
    if p.boss == true then
      love.graphics.print("BOSS INSIDE !",p.x,p.y)
    end
  end
  
  donjon.DessineMapDonjon(salleCourante.salle)
    
  spriteManager.draw()
  mobManager.draw()
  bulletManager.draw()
  particleManager.draw()
  
  --love.graphics.circle("fill",samSprite.x,samSprite.y,5)
  
  -- Affiche des infos de debug sur la salle
  local sDebug = "Infos:"
  if salleCourante.salle.dropCle then
    sDebug = sDebug .. " (Salle de la clé) "
  end
  sDebug = sDebug .. " (Type de pattern="..tostring(salleCourante.salle.mobPattern)..") "
  
  love.graphics.print(sDebug,5,4*HAUTEURTILE)
      
end

function love.keypressed(key)
  if key == "space" or key == " " then
    DemarreJeu()
  end
end

function love.mousepressed(x, y, button, istouch)
  if button == 1 then -- the primary button
    local dx = x/3 - samSprite.x
    local dy = y/3 - samSprite.y
    
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