const fs = require('fs');

const data = fs.readFileSync('../data/campus_data.txt', 'utf8');
const lines = data.split('\n');

let isNode = false;
let isEdge = false;

const nodes = [];
const edges = [];

for (let line of lines) {
    line = line.trim();
    if (!line) continue;
    if (line.startsWith('# NODES')) {
        isNode = true;
        isEdge = false;
        continue;
    }
    if (line.startsWith('# EDGES')) {
        isNode = false;
        isEdge = true;
        continue;
    }

    if (isNode) {
        const parts = line.split(' ');
        const id = parseInt(parts[0]);
        const x = parseFloat(parts[1]);
        const y = parseFloat(parts[2]);
        const name = parts.slice(3).join(' ');
        
        // Transform X, Y to Lat, Lng
        // Assuming we want the map to look somewhat like the graph. 
        // We'll flip Y so graph +Y goes Down (South) if we use standard screen coordinates, but if it was standard math coordinates +Y goes Up (North).
        // Let's assume +Y means South like a web canvas, so higher Y means lower Lat.
        const lat = 30.7680 - (y - 10) * 0.00015; 
        const lng = 76.5750 + (x - 40) * 0.00015; 
        
        nodes.push({ id, name, lat: Number(lat.toFixed(5)), lng: Number(lng.toFixed(5)) });
    }

    if (isEdge) {
        const parts = line.split(' ');
        const u = parseInt(parts[0]);
        const v = parseInt(parts[1]);
        const dist = parseFloat(parts[2]);
        edges.push({ u, v, dist });
    }
}

let output = `export const nodes = [\n`;
output += nodes.map(n => `  { id: ${n.id}, name: "${n.name}", lat: ${n.lat}, lng: ${n.lng} }`).join(',\n');
output += `\n];\n\n`;

output += `export const edges = [\n`;
output += edges.map(e => `  { u: ${e.u}, v: ${e.v}, dist: ${e.dist} }`).join(',\n');
output += `\n];\n`;

fs.writeFileSync('CampusData.js', output);
console.log('Updated CampusData.js');
