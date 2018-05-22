clear; close all; clc;

vertex = [
+0.7000,+0.0000,+0.7331;
+0.3500,+0.6062,+0.7331;
-0.3500,+0.6062,+0.7331;
-0.7000,-0.0000,+0.7331;
-0.3500,-0.6062,+0.7331;
+0.3500,-0.6062,+0.7331;
+1.0000,+0.0000,+0.0000;
+0.5000,+0.8660,+0.0000;
-0.5000,+0.8660,+0.0000;
-1.0000,-0.0000,+0.0000;
-0.5000,-0.8660,+0.0000;
+0.5000,-0.8660,+0.0000;
+1.0000,+0.0000,-0.0000;
+0.5000,+0.8660,-0.0000;
-0.5000,+0.8660,-0.0000;
-1.0000,-0.0000,-0.0000;
-0.5000,-0.8660,-0.0000;
+0.5000,-0.8660,-0.0000;
+0.1000,+0.0000,-1.4661;
+0.0500,+0.0866,-1.4661;
-0.0500,+0.0866,-1.4661;
-0.1000,-0.0000,-1.4661;
-0.0500,-0.0866,-1.4661;
+0.0500,-0.0866,-1.4661;
];
face = [
0,1,2;
0,2,3;
3,4,5;
3,5,0;
0,6,1;
6,7,1;
1,7,2;
7,8,2;
2,8,3;
8,9,3;
3,9,4;
9,10,4;
4,10,5;
10,11,5;
5,11,0;
11,6,0;
6,12,7;
12,13,7;
7,13,8;
13,14,8;
8,14,9;
14,15,9;
9,15,10;
15,16,10;
10,16,11;
16,17,11;
11,17,6;
17,12,6;
12,18,13;
18,19,13;
13,19,14;
19,20,14;
14,20,15;
20,21,15;
15,21,16;
21,22,16;
16,22,17;
22,23,17;
17,23,12;
23,18,12;
18,20,19;
18,21,20;
21,23,22;
21,18,23;
];

figure(1); clf;
hold on;
axis equal;
for i = 1:size(face,1)
    v = vertex(face(i, :)+1, :);
    plot3([v(1,1), v(2,1)], [v(1,2), v(2,2)], [v(1,3), v(2,3)], ...
        'k', 'linewidth', 1.5);
    plot3([v(2,1), v(3,1)], [v(2,2), v(3,2)], [v(2,3), v(3,3)], ...
        'k', 'linewidth', 1.5);
    plot3([v(3,1), v(1,1)], [v(3,2), v(1,2)], [v(3,3), v(1,3)], ...
        'k', 'linewidth', 1.5);
end