-- Cette ligne permet d'afficher des traces dans la console pendant l'éxécution
io.stdout:setvbuf('no')

-- Empèche Love de filtrer les contours des images quand elles sont redimentionnées
-- Indispensable pour du pixel art
love.graphics.setDefaultFilter("nearest")

-- Cette ligne permet de déboguer pas à pas dans ZeroBraneStudio
if arg[#arg] == "-debug" then require("mobdebug").start() end

heros = {}

math.randomseed(love.timer.getTime())

-- Listes d'éléments
liste_sprites = {}
liste_tirs = {}
liste_aliens = {}

-- Niveau 16x12
map = require("map")
niveau = map.layers[1].data

-- Camera
camera = {}
camera.y = 0
camera.vitesse = 1

-- Ecran courant
ecran_courant = "menu"

victoire = false
timerVictoire = 0

-- Images des tuiles
imgTuiles = {}
local n
for n=1,3 do
  imgTuiles[n] = love.graphics.newImage("images/tuile_"..n..".png")
end

-- Images des explosions
imgExplosion = {}
for n=1,5 do
  imgExplosion[n] = love.graphics.newImage("images/explode_"..n..".png")
end

imgMenu = love.graphics.newImage("images/menu.jpg")
imgGameover = love.graphics.newImage("images/gameover.jpg")
imgVictoire = love.graphics.newImage("images/victory.jpg")

sonShoot = love.audio.newSource("sons/shoot.wav","static")
sonExplode = love.audio.newSource("sons/explode_touch.wav","static")

function math.angle(x1,y1, x2,y2) return math.atan2(y2-y1, x2-x1) end

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

function CreeTir(pType, pNomImage, pX, pY, pVitesseX, pVitesseY)
    local tir = CreeSprite(pNomImage, pX, pY)
    tir.type = pType
    tir.vx = pVitesseX
    tir.vy = pVitesseY
    table.insert(liste_tirs, tir)
    
    sonShoot:play()
end

function CreeAlien(pType, pX, pY)
  
  local nomImage = ""
  if pType == 1 then
    nomImage = "enemy1"
  elseif pType == 2 then
    nomImage = "enemy2"
  elseif pType == 3 then
    nomImage = "tourelle"
  elseif pType == 10 then
    nomImage = "enemy3"
  end
  
  local alien = CreeSprite(nomImage, pX, pY)
  
  alien.type = pType
  
  alien.endormi = true
  alien.chronotir = 0
  
  if pType == 1 then
    alien.vy = 2
    alien.vx = 0
    alien.energie = 1
  elseif pType == 2 then
    alien.vy = 2
    local direction = math.random(1,2)
    if direction == 1 then
      alien.vx = 1
    else
      alien.vx = -1
    end
    alien.energie = 3
  elseif pType == 3 then
    alien.vx = 0
    alien.vy = camera.vitesse
    alien.energie = 5
  elseif pType == 10 then
    alien.vx = 0
    alien.vy = camera.vitesse * 2
    alien.energie = 20
    alien.angle = 0
  end

  table.insert(liste_aliens, alien)
end

function CreeSprite(pNomImage, pX, pY)
  
  sprite = {}
  sprite.x = pX
  sprite.y = pY
  sprite.supprime = false
  sprite.image = love.graphics.newImage("images/"..pNomImage..".png")
  sprite.l = sprite.image:getWidth()
  sprite.h = sprite.image:getHeight()
  
  sprite.frame = 1
  sprite.listeFrames = {}
  sprite.maxFrame = 1
  
  table.insert(liste_sprites, sprite)
  
  return sprite
  
end

function CreeExplosion(pX,pY)
  local nouvExplosion = CreeSprite("explode_1",pX,pY)
  nouvExplosion.listeFrames = imgExplosion
  nouvExplosion.maxFrame = 5
end

function love.load()
  
  love.window.setMode(1024,768)
  love.window.setTitle("Atelier Shooter Gamecodeur")
  
  largeur = love.graphics.getWidth()
  hauteur = love.graphics.getHeight()
  
  heros = CreeSprite("heros", largeur/2, hauteur/2)
  
  DemarreJeu()
    
end

function DemarreJeu()
  heros.x = largeur/2
  heros.y = hauteur - (heros.h*2)
  
  ligne = #niveau
  --CreeAlien(10, largeur/2, -(64/2)-(64*(ligne-1)))
  
  -- RAZ de la Caméra
  camera.y = 0
end

function updateJeu()
    -- Avance la camera
    camera.y = camera.y + camera.vitesse
    
    local n

    -- traitement des tirs
    for n=#liste_tirs,1,-1 do
      local tir = liste_tirs[n]
      tir.x = tir.x + tir.vx
      tir.y = tir.y + tir.vy
      
      -- Vérifie si on touche le heros
      if tir.type == "alien" then
        if collide(heros,tir) then
          print("Boom je suis touché !!")
          tir.supprime = true
          table.remove(liste_tirs, n)
          ecran_courant = "gameover"
        end
      end
      
      if tir.type == "heros" then
        local nAlien
        for nAlien=#liste_aliens,1,-1 do
          local alien = liste_aliens[nAlien]
          if alien.endormi == false then
            if collide(tir,alien) then
              CreeExplosion(tir.x,tir.y)
              tir.supprime = true
              table.remove(liste_tirs, n)
              alien.energie = alien.energie - 1
              sonExplode:play()
              if alien.energie <= 0 then
                local nExplosion
                for nExplosion=1,5 do
                  CreeExplosion(alien.x + math.random(-10,10), alien.y + math.random(-10,10))
                end
                if alien.type == 10 then
                  victoire = true
                  timerVictoire = 200
                  for nExplosion=1,20 do
                    CreeExplosion(alien.x + math.random(-100,100), alien.y + math.random(-100,100))
                  end
                end
                alien.supprime = true
                table.remove(liste_aliens, nAlien)
              end
            end
          end
        end
      end
      
      -- Vérifier si le tir n'est pas sorti de l'écran
      if (tir.y < 0 or tir.y > hauteur) and tir.supprime == false then
        
        -- Marque le sprite pour le supprimer plus tard
        tir.supprime = true
        table.remove(liste_tirs, n)
        
      end
      
    end
    
    -- traitement des aliens
    for n=#liste_aliens,1,-1 do
      local alien = liste_aliens[n]
      
      if alien.y > 0 then
        alien.endormi = false
      end
      
      if alien.endormi == false then
        alien.x = alien.x + alien.vx
        alien.y = alien.y + alien.vy
        
        if alien.type == 1 or alien.type == 2 then
          alien.chronotir = alien.chronotir - 1
          if alien.chronotir <= 0 then
            alien.chronotir = math.random(60,100)
            CreeTir("alien","laser2",alien.x,alien.y,0,10)
          end
        elseif alien.type == 3 then
          alien.chronotir = alien.chronotir - 1
          if alien.chronotir <= 0 then
            alien.chronotir = 40
            local vx,vy
            local angle
            angle = math.angle(alien.x,alien.y,heros.x,heros.y)
            vx = 10 * math.cos(angle)
            vy = 10 * math.sin(angle)
            CreeTir("alien","laser2",alien.x,alien.y,vx,vy)
          end
        elseif alien.type == 10 then
          if alien.y > hauteur/3 then
            alien.y = hauteur/3
          end
          alien.chronotir = alien.chronotir - 1
          if alien.chronotir <= 0 then
            alien.chronotir = 15
            local vx,vy
            alien.angle = alien.angle + 0.5
            vx = 10 * math.cos(alien.angle)
            vy = 10 * math.sin(alien.angle)
            CreeTir("alien","laser2",alien.x,alien.y,vx,vy)
          end
        end
        
      else
        alien.y = alien.y + camera.vitesse
      end
      
      if alien.y > hauteur then
        alien.supprime = true
        table.remove(liste_aliens, n)
      end
    end
    
    -- Traitement et purge des sprites
    for n=#liste_sprites,1,-1 do
      local sprite = liste_sprites[n]
      -- Le sprite est il animé ?
      if sprite.maxFrame > 1 then
        sprite.frame = sprite.frame + 0.2
        if math.floor(sprite.frame) > sprite.maxFrame then
          sprite.supprime = true
        else
          sprite.image = sprite.listeFrames[math.floor(sprite.frame)]
        end
      end
      if sprite.supprime == true then
        table.remove(liste_sprites,n)
      end
    end

    if love.keyboard.isDown("right") and heros.x < largeur then
      heros.x = heros.x + 4
    end

    if love.keyboard.isDown("left") and heros.x > 0 then
      heros.x = heros.x - 4
    end
    
    if love.keyboard.isDown("up") and heros.y > 0 then
      heros.y = heros.y - 4
    end

    if love.keyboard.isDown("down") and heros.y < hauteur then
      heros.y = heros.y + 4
    end
    
    if victoire == true then
      timerVictoire = timerVictoire - 1
      if timerVictoire == 0 then
        ecran_courant = "victoire"
      end
    end
end

function updateMenu()
end

function love.update(dt)
  
  if ecran_courant == "jeu" then
    updateJeu()
  elseif ecran_courant == "menu" then
    updateMenu()
  end

end

function drawJeu()
  
  -- Dessin du niveau
  local nbLignes = #niveau/16
  local ligne,colonne
  local x,y
  
  x=0
  y=(0 - 64) + camera.y
  for ligne=nbLignes,1,-1 do
    for colonne=1,16 do
      -- Dessine la tuile
      local tuile = niveau[ ((ligne-1)*16) + colonne ]
      if tuile > 0 then
        love.graphics.draw(imgTuiles[tuile],x,y,0,2,2)
      end
      x = x + 64
    end
    x = 0
    y = y - 64
  end
  
  local n
  for n=1,#liste_sprites do
    local s = liste_sprites[n]
    love.graphics.draw(s.image, s.x, s.y, 0, 2, 2, s.l/2, s.h/2)
  end
  
  love.graphics.print("Nombre de tirs = "..#liste_tirs.." Nombre de sprites = "..#liste_sprites.." Nombre d'aliens = "..#liste_aliens,0,0)
 
end

function drawMenu()
  love.graphics.draw(imgMenu,0,0)
end

function drawGameover()
  love.graphics.draw(imgGameover,0,0)
end

function drawVictoire()
  love.graphics.draw(imgVictoire,0,0)
end

function love.draw()
  
  if ecran_courant == "jeu" then
    drawJeu()
  elseif ecran_courant == "menu" then
    drawMenu()
  elseif ecran_courant == "gameover" then
    drawGameover()
  elseif ecran_courant == "victoire" then
    drawVictoire()
  end
   
end

function love.keypressed(key)
  
  if ecran_courant == "jeu" then
    if key == "space" then
      CreeTir("heros", "laser1", heros.x, heros.y - (heros.h*2)/2, 0, -10)
    end
  elseif ecran_courant == "menu" then
    if key == "space" then
      ecran_courant = "jeu"
    end
  end
  
end
  