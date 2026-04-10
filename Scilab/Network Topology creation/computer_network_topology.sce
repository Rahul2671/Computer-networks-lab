clc;
clear;
close;

n = 25;

edges = [
1 2; 1 3; 1 4; 1 5; 1 6; 1 7; 1 8;
9 10; 10 11; 11 12; 12 13; 13 14; 14 15; 15 16; 16 9;
17 18; 17 19; 18 20; 18 21; 19 22; 19 23; 21 24; 21 25;
8 9;
16 17
];

coords = rand(2, n);

scf(1);
clf();
a = gca();
a.data_bounds = [0 0; 1.2 1.2];
a.box = "on";

// ---- DRAW EDGES WITH COLORS AND NUMBERS ----
for k = 1:size(edges,1)
    i = edges(k,1);
    j = edges(k,2);

    xpoly([coords(1,i) coords(1,j)], [coords(2,i) coords(2,j)]);
    e = gce();

    if (i <= 8 & j <= 8) then
        e.foreground = 2;        // Star edges
    elseif (i >= 9 & i <= 16) then
        e.foreground = 3;        // Ring edges
    elseif (i >= 17) then
        e.foreground = 4;        // Tree edges
    else
        e.foreground = 5;        // Hybrid links
    end

    xm = (coords(1,i) + coords(1,j)) / 2;
    ym = (coords(2,i) + coords(2,j)) / 2;
    xstring(xm, ym, string(k));
end

// ---- DRAW NODES WITH COLORS AND NUMBERS ----
for i = 1:n
    xfarc(coords(1,i)-0.015, coords(2,i)-0.015, 0.03, 0.03, 0, 360*64);
    c = gce();

    if i <= 8 then
        c.foreground = 2;        // Star nodes
    elseif i <= 16 then
        c.foreground = 3;        // Ring nodes
    else
        c.foreground = 4;        // Tree nodes
    end

    xstring(coords(1,i)+0.02, coords(2,i)+0.02, string(i));
end

// ---- DEGREE CALCULATION ----
deg = zeros(1,n);
for k = 1:size(edges,1)
    deg(edges(k,1)) = deg(edges(k,1)) + 1;
    deg(edges(k,2)) = deg(edges(k,2)) + 1;
end

disp("Node    Number of Edges");
for i = 1:n
    disp([i deg(i)]);
end

[maxEdges, maxNode] = max(deg);
disp("Node with maximum edges:");
disp(maxNode);
disp("Maximum edges:");
disp(maxEdges);

disp("Total number of nodes:");
disp(n);
disp("Total number of edges:");
disp(size(edges,1));
