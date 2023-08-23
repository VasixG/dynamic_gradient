const canvas = document.getElementById('canvas');
const ctx = canvas.getContext('2d');

// Define the size of each grid cell.
const cellSize = 6;
const rows = canvas.height / cellSize;
const cols = canvas.width / cellSize;
canvas.style.webkitFilter = "blur(10px)";
let alignx =  canvas.width / 2;
let aligny =  canvas.width / 2;

Module.onRuntimeInitialized = function() {
    function draw(t){
        for (let i = 0; i < cols; i++) {
            for (let j = 0; j < rows; j++) {
                const x = i * cellSize- alignx;
                const y = j * cellSize- aligny;
          
                const ptr = Module._interpolateColor(x, y, t);
    
                const r = Module.HEAPF64[ptr / 8];
                const g = Module.HEAPF64[(ptr + 8) / 8];
                const b = Module.HEAPF64[(ptr + 16) / 8];
    
                ctx.fillStyle = `rgb(${Math.round(r)}, ${Math.round(g)}, ${Math.round(b)})`;
                ctx.fillRect(x+ alignx,y+ aligny, cellSize, cellSize);
                Module._free_memory(ptr);
            }
        }
    }

    let startTime;
    function animate(timestamp) {
        if (startTime === undefined) {
            startTime = timestamp;
        }

        // Calculate elapsed time in seconds
        const elapsedTime = (timestamp - startTime) / 15000.0;

        // Clear canvas
        ctx.clearRect(0, 0, canvas.width, canvas.height);

        // Call draw function with elapsedTime as parameter
        draw(elapsedTime);

        // Request next frame
        requestAnimationFrame(animate);
    }
    //draw(0);
    requestAnimationFrame(animate);
};
