local donjon = {}
donjon.nombreDeColonnes = 9
donjon.nombreDeLignes = 6
donjon.salleDepart = nil
donjon.map = {}

function CreeSalle(pLigne,pColonne)
  local newSalle = {}
  
  newSalle.ligne = pLigne
  newSalle.colonne = pColonne
  
  newSalle.estOuverte = false
  
  newSalle.porteHaut = false
  newSalle.porteDroite = false
  newSalle.porteBas = false
  newSalle.porteGauche = false
  
  return newSalle
end

function genereDonjon()
  donjon.map = {}
  
  local nLigne,nColonne
  for nLigne=1,donjon.nombreDeLignes do
    donjon.map[nLigne] = {}
    for nColonne=1,donjon.nombreDeColonnes do
      donjon.map[nLigne][nColonne] = CreeSalle(nLigne,nColonne)
    end
  end
  
  -- Génère le donjon
  
  local listeSalles = {}
  local nbSalles = 20
  
  -- Crée la salle de départ
  local nLigneDepart,nColonneDepart
  nLigneDepart = math.random(1, donjon.nombreDeLignes)
  nColonneDepart = math.random(1, donjon.nombreDeColonnes)
  local salleDepart = donjon.map[nLigneDepart][nColonneDepart]
  salleDepart.estOuverte = true
  table.insert(listeSalles,salleDepart)
  -- Mémoriser la salle de départ
  donjon.salleDepart = salleDepart
  
  while #listeSalles < nbSalles do
    
    -- Sélectionne une salle dans la liste
    local nSalle = math.random(1, #listeSalles)
    local nLigne = listeSalles[nSalle].ligne
    local nColonne = listeSalles[nSalle].colonne
    local salle = listeSalles[nSalle]
    local nouvelleSalle = nil
    
    local direction = math.random(1,4)
    
    local bAjouteSalle = false

    if direction == 1 and nLigne > 1 then
      -- Haut
      nouvelleSalle = donjon.map[nLigne-1][nColonne]
      if nouvelleSalle.estOuverte == false then
        salle.porteHaut = true
        nouvelleSalle.porteBas = true
        bAjouteSalle = true
      end
    elseif direction == 2 and nColonne < donjon.nombreDeColonnes then
      -- Droite
      nouvelleSalle = donjon.map[nLigne][nColonne+1]
      if nouvelleSalle.estOuverte == false then
        salle.porteDroite = true
        nouvelleSalle.porteGauche = true
        bAjouteSalle = true
      end
    elseif direction == 3 and nLigne < donjon.nombreDeLignes then
      -- Bas
      nouvelleSalle = donjon.map[nLigne+1][nColonne]
      if nouvelleSalle.estOuverte == false then
        salle.porteBas = true
        nouvelleSalle.porteHaut = true
        bAjouteSalle = true
      end
    elseif direction == 4 and nColonne > 1 then
      -- Gauche
      nouvelleSalle = donjon.map[nLigne][nColonne-1]
      if nouvelleSalle.estOuverte == false then
        salle.porteGauche = true
        nouvelleSalle.porteDroite = true
        bAjouteSalle = true
      end
    end
    
    -- Ajoute la salle
    if bAjouteSalle == true then
        nouvelleSalle.estOuverte = true
        table.insert(listeSalles, nouvelleSalle)      
    end
    
  end
  
end

function love.load()
  genereDonjon()
end

function love.update(dt)
end

function love.draw()
  local x,y
  x=5
  y=5
  local largeurCase = 34
  local hauteurCase = 13
  local espaceCase = 5
  local nLigne,nColonne
  for nLigne=1,donjon.nombreDeLignes do
    x = 5
    for nColonne=1,donjon.nombreDeColonnes do
      if donjon.map[nLigne][nColonne].estOuverte == false then
        love.graphics.setColor(60,60,60)
        love.graphics.rectangle("fill",x,y,largeurCase,hauteurCase)
      else
        if donjon.salleDepart == donjon.map[nLigne][nColonne] then
          love.graphics.setColor(25,255,25)
        else
          love.graphics.setColor(255,255,255)
        end
        love.graphics.rectangle("fill",x,y,largeurCase,hauteurCase)
      end
      x = x + largeurCase + espaceCase
    end
    y = y + hauteurCase + espaceCase
  end
end

function love.keypressed(key)
  if key == "space" or key == " " then
    genereDonjon()
  end
end