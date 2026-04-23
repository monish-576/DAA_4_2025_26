const fs = require('fs');

const rawNodes = [
  { id: 1, name: "Administration/South Block", lat: 30.7708, lng: 76.5703 },
  { id: 2, name: "Main Library (Block 3)", lat: 30.7682, lng: 76.5762 },
  { id: 3, name: "Academic Block 1", lat: 30.7697, lng: 76.5755 },
  { id: 4, name: "Academic Block 2", lat: 30.7692, lng: 76.5758 },
  { id: 5, name: "Academic Block 4", lat: 30.7673, lng: 76.5746 },
  { id: 6, name: "Academic Block 5", lat: 30.7663, lng: 76.5760 },
  { id: 7, name: "Nek Chand Boys Hostel", lat: 30.7641, lng: 76.5744 },
  { id: 8, name: "Zakir Boys Hostel", lat: 30.7640, lng: 76.5725 },
  { id: 9, name: "Sukhna Girls Hostel", lat: 30.7706, lng: 76.5778 },
  { id: 10, name: "Tagore Girls Hostel", lat: 30.7654, lng: 76.5759 },
  { id: 11, name: "Sports Ground", lat: 30.7680, lng: 76.5735 },
  { id: 12, name: "Lords Food Court", lat: 30.7671, lng: 76.5761 },
  { id: 13, name: "Auditorium (Block 6)", lat: 30.7669, lng: 76.5761 },
  { id: 14, name: "LC Girls Hostel", lat: 30.7704, lng: 76.5732 },
  { id: 15, name: "Shivalik Hostel", lat: 30.7711, lng: 76.5688 },
  { id: 16, name: "HR Cafe", lat: 30.7712, lng: 76.5702 },
  { id: 17, name: "Block 9", lat: 30.7718, lng: 76.5783 },
  { id: 18, name: "Block 7", lat: 30.7690, lng: 76.5785 }
];

function haversine(lat1, lon1, lat2, lon2) {
  const R = 6371e3; // metres
  const φ1 = lat1 * Math.PI/180;
  const φ2 = lat2 * Math.PI/180;
  const Δφ = (lat2-lat1) * Math.PI/180;
  const Δλ = (lon2-lon1) * Math.PI/180;

  const a = Math.sin(Δφ/2) * Math.sin(Δφ/2) +
            Math.cos(φ1) * Math.cos(φ2) *
            Math.sin(Δλ/2) * Math.sin(Δλ/2);
  const c = 2 * Math.atan2(Math.sqrt(a), Math.sqrt(1-a));

  return Math.round(R * c);
}

// Generate edges (3 nearest neighbors to ensure connectedness without a messy complete graph)
const edgesMap = new Map();

for (const n1 of rawNodes) {
    // calculate all dists
    const dists = [];
    for (const n2 of rawNodes) {
        if (n1.id === n2.id) continue;
        dists.push({ id: n2.id, dist: haversine(n1.lat, n1.lng, n2.lat, n2.lng) });
    }
    
    // sort by dist
    dists.sort((a,b) => a.dist - b.dist);
    
    // Connect to 3 closest
    for(let i=0; i<4; i++) {
        if (!dists[i]) break;
        const target = dists[i];
        const u = Math.min(n1.id, target.id);
        const v = Math.max(n1.id, target.id);
        const key = u + '-' + v;
        if (!edgesMap.has(key)) {
            edgesMap.set(key, { u, v, dist: target.dist });
        }
    }
}

const edgesArr = Array.from(edgesMap.values());

let output = `export const nodes = [\n`;
output += rawNodes.map(n => `  { id: ${n.id}, name: "${n.name}", lat: ${n.lat}, lng: ${n.lng} }`).join(',\n');
output += `\n];\n\n`;

output += `export const edges = [\n`;
output += edgesArr.map(e => `  { u: ${e.u}, v: ${e.v}, dist: ${e.dist} }`).join(',\n');
output += `\n];\n`;

fs.writeFileSync('CampusData.js', output);
console.log('Updated CampusData.js with real coords and nearest-neighbor paths!');
