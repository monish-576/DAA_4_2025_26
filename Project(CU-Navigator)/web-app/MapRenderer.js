import L from 'leaflet';
import 'leaflet/dist/leaflet.css';
import { nodes, edges } from './CampusData.js';

export class MapRenderer {
  constructor(mapId, graph) {
    this.graph = graph;
    this.activePathIds = [];
    this.polylines = [];
    this.markerObjects = {};
    this.onNodeClick = null;
    this.isDarkMode = true;

    // Map themes
    this.darkLayer = L.tileLayer('https://{s}.basemaps.cartocdn.com/dark_all/{z}/{x}/{y}{r}.png', {
      maxZoom: 20
    });
    
    this.streetLayer = L.tileLayer('https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png', {
      maxZoom: 20
    });

    this.map = L.map(mapId, { zoomControl: false }).setView([30.7680, 76.5750], 16);
    this.darkLayer.addTo(this.map); // default

    this.drawBaseMap();
  }

  toggleTheme() {
    this.isDarkMode = !this.isDarkMode;
    if (this.isDarkMode) {
      this.map.removeLayer(this.streetLayer);
      this.darkLayer.addTo(this.map);
    } else {
      this.map.removeLayer(this.darkLayer);
      this.streetLayer.addTo(this.map);
    }
  }

  drawBaseMap() {
    nodes.forEach(n => {
        const marker = L.circleMarker([n.lat, n.lng], {
            radius: 8,
            fillColor: '#8b949e',
            color: '#000',
            weight: 2,
            opacity: 1,
            fillOpacity: 0.9
        }).addTo(this.map);

        marker.bindTooltip(n.name, {
            permanent: false,
            direction: 'top',
            className: 'custom-tooltip'
        });

        marker.on('click', () => {
            if(this.onNodeClick) this.onNodeClick(n.id);
        });

        this.markerObjects[n.id] = marker;
    });
  }

  async fetchOSRMFootpath(startLat, startLng, endLat, endLng) {
    const url = `https://router.project-osrm.org/route/v1/foot/${startLng},${startLat};${endLng},${endLat}?geometries=geojson`;
    try {
        const response = await fetch(url);
        const data = await response.json();
        if (data.routes && data.routes.length > 0) {
            // OSRM returns GeoJSON coordinates as [lng, lat], Leaflet wants [lat, lng]
            return data.routes[0].geometry.coordinates.map(coord => [coord[1], coord[0]]);
        }
    } catch (e) {
        console.error("OSRM Routing failed:", e);
    }
    // Fallback to straight line
    return [[startLat, startLng], [endLat, endLng]];
  }

  async setActivePath(pathParams) {
    this.activePathIds = pathParams;
    await this.updateRender();
  }

  async updateRender() {
    this.polylines.forEach(p => this.map.removeLayer(p));
    this.polylines = [];

    Object.keys(this.markerObjects).forEach(id => {
       this.markerObjects[id].setStyle({ fillColor: '#8b949e', radius: 8 });
    });

    if(this.activePathIds.length === 0) return;

    let fullPathLatLngs = [];

    // Highlight markers and build path
    for(let i=0; i<this.activePathIds.length; i++) {
        const id = this.activePathIds[i];
        const n = this.graph.getNode(id);
        if(!n) continue;

        const marker = this.markerObjects[id];
        marker.setStyle({ fillColor: '#58a6ff', radius: 10 });

        if(i === 0) marker.setStyle({ fillColor: '#3fb950', radius: 12 });
        else if (i === this.activePathIds.length - 1) marker.setStyle({ fillColor: '#f85149', radius: 12 });

        if(i > 0) {
            const prev = this.graph.getNode(this.activePathIds[i-1]);
            // Fetch human walking path from OSRM between these node segments
            const segmentCoords = await this.fetchOSRMFootpath(prev.lat, prev.lng, n.lat, n.lng);
            fullPathLatLngs.push(...segmentCoords);
        }
    }

    if(fullPathLatLngs.length === 0) return;

    // Glowing curved human route
    const glowLine = L.polyline(fullPathLatLngs, {
        color: '#58a6ff',
        weight: 10,
        opacity: 0.3,
        lineJoin: 'round',
        lineCap: 'round',
        smoothFactor: 1
    }).addTo(this.map);

    const solidLine = L.polyline(fullPathLatLngs, {
        color: '#58a6ff',
        weight: 4,
        opacity: 1,
        lineJoin: 'round',
        lineCap: 'round',
        smoothFactor: 1
    }).addTo(this.map);

    this.polylines.push(glowLine, solidLine);

    // Zoom
    const bounds = solidLine.getBounds();
    if(bounds.isValid()) {
      this.map.flyToBounds(bounds, { padding: [50, 50], duration: 1.5 });
    }
  }
}
