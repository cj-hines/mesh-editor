<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">
<head>
<style>
  body {
    background-color: white;
    padding: 100px;
    width: 1000px;
    margin: auto;
    text-align: left;
    font-weight: 300;
    font-family: 'Open Sans', sans-serif;
    color: #121212;
  }
  h1, h2, h3, h4 {
    font-family: 'Source Sans Pro', sans-serif;
  }
  kbd {
    color: #121212;
  }
</style>
<title>CS 184 Mesh Edit</title>
<meta http-equiv="content-type" content="text/html; charset=utf-8" />
<link href="https://fonts.googleapis.com/css?family=Open+Sans|Source+Sans+Pro" rel="stylesheet">

<script>
  MathJax = {
    tex: {
      inlineMath: [['$', '$'], ['\\(', '\\)']]
    }
  };
</script>
<script id="MathJax-script" async
  src="https://cdn.jsdelivr.net/npm/mathjax@3/es5/tex-chtml.js">
</script>

</head>


<body>

<h1 align="middle">CS 184: Computer Graphics and Imaging, Spring 2023</h1>
<h1 align="middle">Mesh Edit</h1>
<h2 align="middle">CJ Hines</h2>

<br><br>

<div align="middle">
  <table style="width:100%">
    <tr align="center">
      <td>
        <img src="docs/4.1.png" align="middle" width="400px"/>
      </td>
    </tr>
  </table>
</div>

<div>

<h2 align="middle">Overview</h2>
<p>
    I implemented a mesh editor that allows us to flip edges, split edges, and subdivide the mesh. I also implemented a method for evaluating Bezier curves and surfaces. I learned more about manipulating meshes and the half-edge data structure overall. I ran into multiple bugs which forced me to spend a lot of time with the half edge data structure.
</p>
<br>


<h2 align="middle">Section I: Bezier Curves and Surfaces</h2>

<h3 align="middle">Part 1: Bezier Curves with 1D de Casteljau Subdivision</h3>


<b>
  <u>Briefly explain de Casteljau's algorithm and how you implemented it in order to evaluate Bezier curves.</u>
</b>
<p>
    De Casteljau's algorithm says that given n points we can calculate points between i and i+1 where i+1 < n using linear interpolation and recursively repeat until we've calculated one point along the bezier curve. I implemented a recursive function that calculated the lerp function for every point and returned the point vector or the last point on the bezier curve. 
</p>
<br>


<b>
    <u>Take a look at the provided .bzc files and create your own Bezier curve with 6 control points of your choosing. Use this Bezier curve for your screenshots below.</u>
</b>
<p>
    My 6 point curve is in my_curve.bcz and my screenshots are below.
</p>
<!-- Example of including a single figure -->
<div align="middle">
  <img src="docs/1.6.png" align="middle" width="50%">
</div>
<br>

<b><u>
  Show screenshots of each step / level of the evaluation from the original control points down to the final evaluated point. Press <kbd>E</kbd> to step through. Toggle <kbd>C</kbd> to show the completed Bezier curve as well.
</u></b>
<!-- Example of including multiple figures -->
<div align="middle">
  <table style="width:100%">
    <tr align="center">
      <td>
        <img src="docs/1.0.png" align="middle" width="400px"/>
        <figcaption>Level 0</figcaption>
      </td>
      <td>
        <img src="docs/1.1.png" align="middle" width="400px"/>
        <figcaption>Level 1</figcaption>
      </td>
    </tr>
    <br>
    <tr align="center">
       <td>
        <img src="docs/1.2.png" align="middle" width="400px"/>
        <figcaption>Level 2</figcaption>
      </td>
      <td>
        <img src="docs/1.3.png" align="middle" width="400px"/>
        <figcaption>Level 3</figcaption>
      </td>
    </tr>
    <br>
    <tr align="center">
      <td>
        <img src="docs/1.4.png" align="middle" width="400px"/>
        <figcaption>Level 4</figcaption>
      </td>
      <td>
        <img src="docs/1.6.png" align="middle" width="400px"/>
        <figcaption>Level 5 with the completed bezier curve</figcaption>
      </td>
    </tr>
  </table>
</div>
<br>


<b>
  <u>Show a screenshot of a slightly different Bezier curve by moving the original control points around and modifying the parameter \(t\) via mouse scrolling.</u>
</b>
<div align="middle">
  <table style="width:100%">
    <tr align="center">
      <td>
        <img src="docs/1.7.png" align="middle" width="400px"/>
        <figcaption>Level 5 of a different bezier curve with different control points and different t parameter. </figcaption>
      </td>
    </tr>
  </table>
</div>
<br>  
<br>


<h3 align="middle">Part 2: Bezier Surfaces with Separable 1D de Casteljau</h3>
<b>
  <u>Briefly explain how de Casteljau algorithm extends to Bezier surfaces and how you implemented it in order to evaluate Bezier surfaces.</u>
</b>
<p>
    De Casteljau's algorithm extends to bezier sufraces because we can apply the algorithm recursively to curves on the surface. First, I implemented BezierPatch::evaluateStep() to evaluate one step of the de Casteljau's algorithm like in Part 1. I implemented BezierPatch::evaluate1D() to apply the algorithm to find one point on an array recursively. I implemented BezierPatch::evaluate() to evaluate de Casteljau's for each row and column of control points to evaluate one point on the Bezier surface. By recursively applying the 1D de Casteljau algorithm to each row and column, we can efficiently evaluate any point on the Bezier surface.
</p>
<br>


<b>
  <u>Show a screenshot of bez/teapot.bez (not .dae) evaluated by your implementation.</u>
</b>
<div align="middle">
  <table style="width:100%">
    <tr align="center">
      <td>
        <img src="docs/p2.png" align="middle" width="400px"/>
        <figcaption> Screenshot of bez/teapot.bez. </figcaption>
      </td>
    </tr>
  </table>
</div>
<br>


<h2 align="middle">Section II: Triangle Meshes and Half-Edge Data Structure</h2>

<h3 align="middle">Part 3: Area-Weighted Vertex Normals</h3>

<b>
  <u>Briefly explain how you implemented the area-weighted vertex normals.</u>
</b>
<p>
    I referenced the structure in halfEdgeMesh.cpp for Vertex::normal(). I implemented the area-weighted vertex normals by first initializing a normal vector to (0, 0, 0) and a halfedge. I traversed each triangle in the mesh and calculated its normal vector by computing the cross product of two of its edges. I accumulated each triangle's normal vector into the normal vectors of its vertices. For each triangle, add its normal vector to the normal vectors of its three vertices. Normalize the normal vectors of each vertex by dividing them by their magnitude. For each vertex, compute the weighted average of the normal vectors of its neighboring vertices, using the areas of the adjacent triangles as the weights. 
</p>
<br>


<b>
  <u>Show screenshots of dae/teapot.dae (not .bez) comparing teapot shading with and without vertex normals. Use <kbd>Q</kbd> to toggle default flat shading and Phong shading.</u>
</b>
<p>
<div align="middle">
  <table style="width:100%">
    <tr align="center">
      <td>
        <img src="docs/3.1.png" align="middle" width="400px"/>
        <figcaption> dae/teapot.dae. </figcaption>
      </td>
      <td>
        <img src="docs/3.2.png" align="middle" width="400px"/>
        <figcaption> dae/teapot.dae. </figcaption>
      </td>
    </tr>
  </table>
</div>
</p>
<br>


<h3 align="middle">Part 4: Edge Flip</h3>
<b>
  <u>Briefly explain how you implemented the edge flip operation and describe any interesting implementation / debugging tricks you have used.</u>
</b>
<p> 
    If the edge is not a boundary edge, I can identify all halfedges, vertices, edges, and faces of the two triangles I'm edge flipping. Then I updated all of the pointers with the flipped edge. It was very useful for me to draw the triangles and visualize the pointers like in the picture below.
</p>
<div align="middle">
  <table style="width:100%">
    <tr align="center">
      <td>
        <img src="docs/screenshot.png" align="middle" width="400px"/>
        <figcaption> Visualizing the triangle pointers. </figcaption>
      </td>
    </tr>
  </table>
</div>
<br>


<b>
  <u>Show screenshots of the teapot before and after some edge flips.</u>
</b>
<p>
<div align="middle">
  <table style="width:100%">
    <tr align="center">
      <td>
        <img src="docs/4.1.png" align="middle" width="400px"/>
        <figcaption> teapot 0 edge flips </figcaption>
      </td>
      <td>
        <img src="docs/4.2.png" align="middle" width="400px"/>
        <figcaption> teapot 1 edge flip </figcaption>
      </td>
    <tr align="center">
      <td>
        <img src="docs/4.3.png" align="middle" width="400px"/>
        <figcaption> teapot more edge flips </figcaption>
      </td>
    </tr>
    </tr>
  </table>
</div>
</p>
<br>


<b>
    <u>Write about your eventful debugging journey, if you have experienced one.</u>
</b>
<p>
    Once I drew out the before and after of every element for the edge flip, then every assignment became much more straightforward.
</p>
<br>


<h3 align="middle">Part 5: Edge Split</h3>
<b>
  <u>Briefly explain how you implemented the edge split operation and describe any interesting implementation / debugging tricks you have used.</u>
</b>
<p>
    This implementation was similar to the implementation of edge flip, except I needed to add new elements because when we split the edge we need two new faces, three edges, a vertex, and 6 halfedges. If the edge is not a boundary edge, I can identify all halfedges, vertices, edges, and faces of the two triangles I'm edge splitting. I create a new vertex at the midpoint of the edge. Create two new triangles by connecting the new vertex to the two endpoints of the original edge, and to the third vertex of each of the adjacent triangles. Then I update all of the triangle pointers involved in the split.
</p>
<br>


<b>
    <u>Show screenshots of a mesh before and after some edge splits.</u>
</b>
<p>
<div align="middle">
  <table style="width:100%">
    <tr align="center">
      <td>
        <img src="docs/5.1.png" align="middle" width="400px"/>
        <figcaption> teapot 0 edge splits </figcaption>
      </td>
      <td>
        <img src="docs/5.2.png" align="middle" width="400px"/>
        <figcaption> teapot 1 edge splits </figcaption>
      </td>
      <tr align="center">
      <td>
        <img src="docs/5.3.png" align="middle" width="400px"/>
        <figcaption> teapot more edge splits </figcaption>
      </td>
    </tr>
    </tr>
  </table>
</div>
</p>
<br>


<b>
    <u>Show screenshots of a mesh before and after a combination of both edge splits and edge flips.</u>
</b>
<p>
<div align="middle">
  <table style="width:100%">
    <tr align="center">
      <td>
        <img src="docs/5b1.png" align="middle" width="400px"/>
        <figcaption> teapot 0 edge flips or splits </figcaption>
      </td>
      <td>
        <img src="docs/5b2.png" align="middle" width="400px"/>
        <figcaption> teapot 1 edge flip 1 edge split </figcaption>
      </td>
    </tr>
      <tr align="center">
      <td>
        <img src="docs/5b3.png" align="middle" width="400px"/>
        <figcaption> teapot 2 edge flips 2 edge splits </figcaption>
      </td>
      <td>
        <img src="docs/5b4.png" align="middle" width="400px"/>
        <figcaption> teapot 3 edge flips 3 edge splits </figcaption>
      </td>
    </tr>
    <tr align="center">
      <td>
        <img src="docs/5b5.png" align="middle" width="400px"/>
        <figcaption> teapot 3 edge flips + splits no grid </figcaption>
      </td>
    </tr>
  </table>
</div>
</p>
<br>

<br>


<h3 align="middle">Part 6: Loop Subdivision for Mesh Upsampling</h3>
<b>
  <u>Briefly explain how you implemented the loop subdivision and describe any interesting implementation / debugging tricks you have used.</u>
</b>
<p>
    I followed the steps provided for loop subdivision. First, to compute and store new positions for all the vertices in the input mesh, I split every edge mesh and summed the original positions of the neighboring vertices to calculate the updated position. I also set vertex isNew to false for vertices in the orignal mesh. Second, I computed the updated and stored vertex positions associated with edges based on loop subdivision in lecture and in the assignment. Then, I split every edge in the mesh, looping only over old edges. I struggled a lot with understanding why I kept causing this to loop. I flipped new edges that connected an old and new vertex, and updated the final vertex position.
</p>
<br>

<b> </b>
<p>
    Here are some pictures of bugs I had. Many issues with element pointers and traversing the mesh in the correct order.
    <div align="middle">
  <table style="width:100%">
    <tr align="center">
      <td>
        <img src="docs/wrong1.png" align="middle" width="400px"/>
      </td>
      <td>
        <img src="docs/wrong2.png" align="middle" width="400px"/>
      </td>
    </tr>
  </table>
</div>
</p>
<br>

<br>

<b>
  <u>Load dae/cube.dae. Perform several iterations of loop subdivision on the cube. Notice that the cube becomes slightly asymmetric after repeated subdivisions. Can you pre-process the cube with edge flips and splits so that the cube subdivides symmetrically? Document these effects and explain why they occur. Also explain how your pre-processing helps alleviate the effects.</u>
</b>
<p>
<div align="middle">
  <table style="width:100%">
    <tr align="center">
      <td>
        <img src="docs/6.1.png" align="middle" width="400px"/>
        <figcaption> 0 iterations of loop subdvision </figcaption>
      </td>
      <td>
        <img src="docs/6.2.png" align="middle" width="400px"/>
        <figcaption> 1 iteration of loop subdvision </figcaption>
      </td>
    </tr>
    <tr align="center">
      <td>
        <img src="docs/6.3.png" align="middle" width="400px"/>
        <figcaption> 2 iterations of loop subdvision </figcaption>
      </td>
      <td>
        <img src="docs/6.4.png" align="middle" width="400px"/>
        <figcaption> 3 iterations of loop subdvision </figcaption>
      </td>
    </tr>
    <tr align="center">
      <td>
        <img src="docs/6.5.png" align="middle" width="400px"/>
        <figcaption> 4 iterations of loop subdvision </figcaption>
      </td>
      <td>
        <img src="docs/6.6.png" align="middle" width="400px"/>
        <figcaption> 5 iterations of loop subdvision no grid </figcaption>
      </td>
    </tr>
  </table>
</div>
<br>  
</p>
<br>


<b>
  <p>Pre-processing alleviates some of the asymmentry that occurs when rounding the cube using loop subdivision. My pre-processing balances the corners and starts with an overall more symmetrical shape so when the triangle divide, it's a more even process.</p>
</b>
<p>
<div align="middle">
  <table style="width:100%">
    <tr align="center">
      <td>
        <img src="docs/6good1.png" align="middle" width="400px"/>
        <figcaption> 0 iterations of loop subdvision WITH cube pre-processing using edge flips and splits. </figcaption>
      </td>
      <td>
        <img src="docs/6good2.png" align="middle" width="400px"/>
        <figcaption> 1 iteration of loop subdvision WITH cube pre-processing using edge flips and splits.  </figcaption>
      </td>
    </tr>
    <tr align="center">
      <td>
        <img src="docs/6good3.png" align="middle" width="400px"/>
        <figcaption> 2 iterations of loop subdvision WITH cube pre-processing using edge flips and splits.  </figcaption>
      </td>
      <td>
        <img src="docs/6good4.png" align="middle" width="400px"/>
        <figcaption> Multiple iterations of loop subdvision WITH cube pre-processing using edge flips and splits. </figcaption>
      </td>
    </tr>
  </table>
</div>
<br>  
</p>
<br>


</body>
</html>
