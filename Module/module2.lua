local aliens = {}
aliens.nombre = 0

print("Module 2 / Chargement - il y a",aliens.nombre,"alien(s)")

function aliens:AjouteAlien()
  self.nombre = self.nombre + 1
  
  print("Module 2 / Il y a maintenant",self.nombre,"alien(s)")
end

return aliens