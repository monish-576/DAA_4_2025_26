import './style.css';
import { nodes, edges } from './CampusData.js';
import { Graph } from './Graph.js';
import { MapRenderer } from './MapRenderer.js';

// Elements
const startSelect = document.getElementById('start-select');
const endSelect = document.getElementById('end-select');
const findBtn = document.getElementById('find-route-btn');
const resultsPanel = document.getElementById('results-panel');
const pathText = document.getElementById('route-path-text');
const distText = document.getElementById('route-distance');
const timeText = document.getElementById('route-time');

let graph;
let renderer;
let selectedStartId = null;
let selectedEndId = null;

const themeToggle = document.getElementById('map-theme-toggle');
const themeText = document.getElementById('theme-text');


function init() {
  // 1. Build Graph
  graph = new Graph();
  nodes.forEach(n => graph.addNode(n.id, n.name, n.lat, n.lng)); // Pass lat, lng
  edges.forEach(e => graph.addEdge(e.u, e.v, e.dist));

  // 2. Initialize Leaflet Map Renderer
  renderer = new MapRenderer('map', graph); // Using #map div

  // 3. Populate select boxes
  nodes.forEach(n => {
    const opt1 = document.createElement('option');
    opt1.value = n.id;
    opt1.textContent = n.name;
    startSelect.appendChild(opt1);

    const opt2 = document.createElement('option');
    opt2.value = n.id;
    opt2.textContent = n.name;
    endSelect.appendChild(opt2);
  });

  // 4. Handle Leaflet Map Clicks
  renderer.onNodeClick = (nodeId) => {
    if (!selectedStartId) {
      selectedStartId = nodeId;
      startSelect.value = nodeId;
    } else if (!selectedEndId && nodeId !== selectedStartId) {
      selectedEndId = nodeId;
      endSelect.value = nodeId;
      findRoute(); // Automatically find route
    } else {
      // Reset
      selectedStartId = nodeId;
      startSelect.value = nodeId;
      selectedEndId = null;
      endSelect.value = "";
      renderer.setActivePath([]);
      hideResults();
    }
  };

  // 5. Handle Select changes
  startSelect.addEventListener('change', (e) => {
    selectedStartId = parseInt(e.target.value);
    renderer.setActivePath([]); 
    hideResults();
  });

  endSelect.addEventListener('change', (e) => {
    selectedEndId = parseInt(e.target.value);
    if(selectedStartId && selectedEndId) {
       findRoute();
    }
  });

  // 6. Handle Button
  findBtn.addEventListener('click', () => {
    findRoute();
  });

  // 7. Theme Toggle
  themeToggle.addEventListener('change', (e) => {
    renderer.toggleTheme();
    themeText.textContent = e.target.checked ? "Standard Map" : "Dark Mode";
  });
}

async function findRoute() {
  if (!selectedStartId || !selectedEndId) {
    alert("Please select both a start and destination.");
    return;
  }
  
  if (selectedStartId === selectedEndId) {
      alert("Start and destination cannot be the same.");
      return;
  }

  findBtn.textContent = "Computing Path...";
  findBtn.disabled = true;

  const result = graph.dijkstra(selectedStartId, selectedEndId);
  
  if (result.found) {
    await renderer.setActivePath(result.path);
    showResults(result);
  } else {
    alert("No path found.");
  }
  
  findBtn.textContent = "Find Footpath Routing";
  findBtn.disabled = false;
}

function showResults(result) {
  // Format Path
  const pathNames = result.path.map(id => graph.getNode(id).name);
  pathText.innerHTML = pathNames.join(" <br/>↓<br/> ");

  distText.textContent = `${result.totalDistance}m`;
  
  // Walk speed 5 km/h -> m/s
  const speedMs = (5 * 1000) / 3600;
  const timeSecs = result.totalDistance / speedMs;
  const mins = Math.floor(timeSecs / 60);
  const secs = Math.floor(timeSecs % 60);
  timeText.textContent = `${mins}m ${secs}s`;

  resultsPanel.classList.remove('hidden');
}

function hideResults() {
  resultsPanel.classList.add('hidden');
}

// Ensure init fires
if (document.readyState === 'loading') {
    document.addEventListener('DOMContentLoaded', init);
} else {
    init();
}
