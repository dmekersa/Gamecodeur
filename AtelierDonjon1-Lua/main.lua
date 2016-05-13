local donjon = {}
donjon.nombreDeColonnes = 9
donjon.nombreDeLignes = 6
donjon.map = {}

function genereDonjon()
  donjon.map = {}
  
  local nLigne,nColonne
  for nLigne=1,donjon.nombreDeLignes do
    donjon.map[nLigne] = {}
    for nColonne=1,donjon.nombreDeColonnes do
      donjon.map[nLigne][nColonne] = nil
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
      if donjon.map[nLigne][nColonne] == nil then
        love.graphics.setColor(60,60,60)
        love.graphics.rectangle("fill",x,y,largeurCase,hauteurCase)
      end
      x = x + largeurCase + espaceCase
    end
    y = y + hauteurCase + espaceCase
  end
end