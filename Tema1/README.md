# Tema 1: EV Charger

## Descriere

Scopul acestei lucrari este de a simula o statie de incarcare pentru un vehicul electric, folosind mai multe LED-uri si butoane. 

### Cerinte: 
1. LED-ul RGB reprezinta disponibilitatea statiei. Daca statia este libera, LED-ul va fi verde, iar daca statia este ocupata, va deveni rosu.

2. LED-urile albastre indica gradul de incarcare al bateriei, pe care il vom simula printr-un loader progresiv (L1 = 25%, L2 = 50%, L3 = 75%, L4 = 100%). Loader-ul se incarca prin aprinderea succesiva a LED-urilor, la un interval fix de 3s. LED-ul care semnifica procentul curent de incarcare va avea starea de clipire, LED-urile din urma lui vor fi aprinse continuu, iar celelalte vor fi stinse.

3. Apasarea scurta a butonului de start va porni incarcarea. Apasarea acestui buton in timpul incarcarii nu va face nimic.

4. Apasarea lunga a butonului de stop va opri incarcarea fortat si va reseta statia la starea libera. Apasarea acestui buton cand statia este libera nu va face nimic.

### Componente:

- <b>4 x LED-uri albastre<b> (pentru a simula procentul de incarcare)
- <b>1 x LED RGB<b> (simuleaza starile liber/ocupat)
- <b>2 x Butoane<b> (folosite pentru startul incarcarii si oprirea acesteia)
- <b>7 x Rezistoare 220Ω<b>
- <b>2 x Rezistoare 1kΩ<b>
- <b>Breadbord<b>
- <b>Linii de legatura<b>
