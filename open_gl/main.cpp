// Include standard headers
#include "GL/glut.h"
#include "quadtree.h"
#include <vector>
#include <random>
#include <iostream>
#include <utility>
#include <memory>
#include <fstream>

using namespace std;

Quadtree::Quadtree(std::shared_ptr<BoundaryBox> BB_init, Quadtree *parent, int _nodeDepth)
{
    northWest = nullptr;
    northEast = nullptr;
    southWest = nullptr;
    southEast = nullptr;

    this->boundary2 = std::move(BB_init);

    if (parent == nullptr)
    {
        this->parent = this;
    }
    else
    {
        this->parent = parent;
    }

    this->nodeDepth = _nodeDepth;
}


void Quadtree::clear(Quadtree* t)
{
    if(t != nullptr)
    {
        clear(t->northEast);
        clear(t->northWest);
        clear(t->southEast);
        clear(t->southWest);

        delete t;	// calls the destructor
    }
}


Quadtree::~Quadtree()
{
    delete northWest;
    delete northEast;
    delete southWest;
    delete southEast;

    northWest = nullptr;
    northEast = nullptr;
    southWest = nullptr;
    southEast = nullptr;
}


void Quadtree::clearNode()
{
    delete northWest;
    delete northEast;
    delete southWest;
    delete southEast;

    northWest = nullptr;
    northEast = nullptr;
    southWest = nullptr;
    southEast = nullptr;
}


// insert one point into the tree. Split the tree and relocate the points ot the node if necessary
bool Quadtree::insert(pt2d insertPt)
{
    // check if the point resides in this node
    if (insertPt.x > boundary2->cx+boundary2->dim or insertPt.x <= boundary2->cx-boundary2->dim or insertPt.y > boundary2->cy+boundary2->dim or insertPt.y <= boundary2->cy-boundary2->dim)
        return false;

    if ((children.size() < maxAmtElements and northWest == nullptr) or this->nodeDepth == maxDepth)	// there is room in the node for this pt. Insert the point only if there is no children node available to sort into or if the maximum depth allowed has been reached
    {
        children.push_back(insertPt);
        return true;
    }

    // maximum amount of pts in this node reached -> split into 4 new nodes
    if (northWest == nullptr)	// this node has not been split yet -> nullptr
    {
        subdivide();

        // remove all points from the parent node, and sort this points into the child nodes
        for (auto & i : children)
        {
            // sort this element into the according quadrant(NE, NW, SE, SW)
            pt2d queryPt(i.x, i.y);	// point to insert into child node

            insert(queryPt);
        }

        for (int i = 0; i < (int)children.size(); i++)
            children.erase(children.begin()+i);
    }

    if (northEast->insert(insertPt))
    {
        return true;
    }

    if (northWest->insert(insertPt))
    {
        return true;
    }

    if (southWest->insert(insertPt))
    {
        return true;
    }

    if (southEast->insert(insertPt))
    {
        return true;
    }

    return false;
}


#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-narrowing-conversions"
// split the current node into four new (children)nodes (increment depth by one)
void Quadtree::subdivide()
{
    if (this->nodeDepth < maxDepth)	// split the node only if the maximum depth has not been reached yet
    {
        // subdivide NW
        std::shared_ptr<BoundaryBox> BB_init_NW(new BoundaryBox(boundary2->cx-boundary2->dim*0.5, boundary2->cy+boundary2->dim*0.5, boundary2->dim*0.5));
        northWest = new Quadtree(std::move(BB_init_NW), this, this->nodeDepth+1);

        // subdivide NE
        std::shared_ptr<BoundaryBox> BB_init_NE(new BoundaryBox(boundary2->cx+boundary2->dim*0.5, boundary2->cy+boundary2->dim*0.5, boundary2->dim*0.5));
        northEast = new Quadtree(std::move(BB_init_NE), this, this->nodeDepth+1);

        // subdivide SE
        std::shared_ptr<BoundaryBox> BB_init_SE(new BoundaryBox(boundary2->cx+boundary2->dim*0.5, boundary2->cy-boundary2->dim*0.5, boundary2->dim*0.5));
        southEast = new Quadtree(std::move(BB_init_SE), this, this->nodeDepth+1);

        // subdivide SW
        std::shared_ptr<BoundaryBox> BB_init_SW(new BoundaryBox(boundary2->cx-boundary2->dim*0.5, boundary2->cy-boundary2->dim*0.5, boundary2->dim*0.5));
        southWest = new Quadtree(std::move(BB_init_SW), this, this->nodeDepth+1);
    }
}
#pragma clang diagnostic pop


// drawing routine (used by traverse_and_draw). Used by traverse_and_draw()
void Quadtree::colorPick(Quadtree *t, float *depthColor, int depthColorLen)
{

    if(t->boundary2)
    {
        if (t->nodeDepth*3+2 > depthColorLen)	// default color when the depth exceeds the available colors from the array
        {
            glColor4f(0.0f, 0.0f, 0.0f, 1.0f);
        }
        else	// pick a color according to the array
        {
            glColor4f(depthColor[t->nodeDepth*3], depthColor[t->nodeDepth*3+1], depthColor[t->nodeDepth*3+2], 1.0f);
        }

        double cross = 1.0/2048;

        double center_x =  (t->boundary2->cx)*cross;
        double center_y = (t->boundary2->cy)*cross;
        double dim = (t->boundary2->dim)*cross;

        glBegin(GL_LINE_STRIP);
            glColor3f(1.0,.0,.0);
            glVertex2f(center_x-dim-.5, center_y-.5);
            glVertex2f(center_x+dim-.5, center_y-.5);
        glEnd();

        glBegin(GL_LINE_STRIP);
            glColor3f(1.0,.0,.0);
            glVertex2f(center_x-.5, center_y-dim-.5);
            glVertex2f(center_x-.5, center_y+dim-.5);
        glEnd();
    }
}

void Quadtree::traverse_and_draw(Quadtree *t, float widthRootNode)
{
    // adjust the height (z-coordinate) of the quadtree
    //float elevate = -10.0;

    // pick the colors according to the depth
    float depthColor [] =
            {
                    1.0f, 0.0f, 0.0f,		// depth 1 = red
                    0.0f, 0.392f, 0.0f,		// depth 2 = darkgreen
                    0.0f, 0.0f, 1.0f,		// depth 3 = blue
                    1.0f, 0.0f, 1.0f,		// depth 4 = purple
                    0.0f, 1.0f, 1.0f, 		// depth 5 = cyan
                    0.294f, 0.0f, 0.51f,	// depth 6 = indigo
                    0.863f, 0.078f, 0.235f,	// depth 7 = Crimson
            };

    glLineWidth(2.0f);

    if (t->northWest != nullptr)
    {
        colorPick(t, depthColor, sizeof(depthColor) / sizeof(*depthColor));
        t->northEast->traverse_and_draw(northEast, widthRootNode);
    }

    if (t->northEast != nullptr)
    {
        colorPick(t, depthColor, sizeof(depthColor) / sizeof(*depthColor));
        t->northWest->traverse_and_draw(northWest, widthRootNode);
    }

    if (t->southEast != nullptr)
    {
        colorPick(t, depthColor, sizeof(depthColor) / sizeof(*depthColor));
        t->southEast->traverse_and_draw(southEast, widthRootNode);
    }

    if (t->southWest != nullptr)
    {
        colorPick(t, depthColor, sizeof(depthColor) / sizeof(*depthColor));
        t->southWest->traverse_and_draw(southWest, widthRootNode);
    }
}


// count the nodes of the tree
int Quadtree::count_nodes(Quadtree *t)
{
    // node has been split
    if (t->northEast != nullptr)
    {
        int nodes_NE = northEast->count_nodes(northEast);
        int nodes_SE = southEast->count_nodes(southEast);
        int nodes_SW = southWest->count_nodes(southWest);
        int nodes_NW = northWest->count_nodes(northWest);
        return nodes_NE + nodes_SE + nodes_SW + nodes_NW;
    }

    return 1;
}


// count the elements residing in the tree
int Quadtree::count_elements(Quadtree *t)
{
    int fetch_elements = 0;

    // node has been split - continue with the recursion
    if (t->northEast != nullptr)
    {
        fetch_elements += northEast->count_elements(northEast);
        fetch_elements += southEast->count_elements(southEast);
        fetch_elements += southWest->count_elements(southWest);
        fetch_elements += northWest->count_elements(northWest);
    }
        // deepest (child)node possible
    else
    {
        if (!t->children.empty())	// there are elements in this node
        {
            fetch_elements += t->children.size();
        }
    }

    return fetch_elements;
}


// fetch the node in which the given element resides
Quadtree *Quadtree::fetch_node(pt2d seekPt)
{
    static Quadtree *ReturnNode;

    // point outside of node
    if (seekPt.x > boundary2->cx+boundary2->dim or seekPt.x <= boundary2->cx-boundary2->dim or seekPt.y > boundary2->cy+boundary2->dim or seekPt.y <= boundary2->cy-boundary2->dim)
    {
    }
        // else -> point is inside of the node
    else
    {
        // deepest node corresponding to this point reached. Return the pointer to this node
        if (northWest == nullptr)
        {
            bool foundNode = false;
            ReturnNode = nullptr;

            for (auto & i : children)
            {
                if (seekPt.x == i.x and seekPt.y == i.y)
                {
                    foundNode = true;
                }
            }

            if (foundNode)
            {
                ReturnNode = this;
            }

            return ReturnNode;
        }
        else
        {
            ReturnNode = northEast->fetch_node(seekPt);
            ReturnNode = northWest->fetch_node(seekPt);
            ReturnNode = southWest->fetch_node(seekPt);
            ReturnNode = southEast->fetch_node(seekPt);
        }
    }

    return ReturnNode;
}


// searches the node in which seekPt resides and returns all the elements from this node
[[maybe_unused]] std::vector<pt2d> Quadtree::fetch_points(pt2d seekPt)
{
    std::vector <pt2d> return_elements;

    // search the node in which the point seekPt resides
    Quadtree *search_node = fetch_node(seekPt);

    // retrieve all the pts from the node
    return_elements.reserve((int)search_node->children.size());
for (auto & i : search_node->children)
    {
        return_elements.push_back(i);
    }

    return return_elements;
}


// remove a single element from the tree
bool Quadtree::delete_element(pt2d deletePt)
{
    // try to locate the node where the point lies
    Quadtree *nodePtReside = fetch_node(deletePt);

    if (nodePtReside == nullptr)   // this element is not in the QT
    {
        return false;
    }
    else
    {
        // retrieve location of deletePt in the children std::vector
        int del_index ;
        bool foundItem = false;

        for (del_index = 0; del_index < (int)nodePtReside->children.size(); del_index++)
        {
            if (deletePt.x == nodePtReside->children[del_index].x and deletePt.y == nodePtReside->children[del_index].y)
            {
                foundItem = true;
                nodePtReside->children.erase(nodePtReside->children.begin()+del_index);
                break;
            }
        }

        // element was not found -> deletion failed
        if (!foundItem)
        {
            return false;
        }

        concatenate_nodes(nodePtReside);
    }

    return true;
}


// auxiliary function used by delete_element(). Used to collapse nodes and redistribute elements after collapsing
void Quadtree::concatenate_nodes(Quadtree *concat_this_node_maybe)
{
    if (concat_this_node_maybe->parent == concat_this_node_maybe)   // point resides in parent -> do nothing
    {
    }
    else
    {
        // Concatenate because all four nodes (3 sibling nodes and the one where the point lies) are empty
        if (concat_this_node_maybe->parent->northEast->northEast == nullptr && concat_this_node_maybe->parent->northWest->northEast == nullptr && concat_this_node_maybe->parent->southEast->northEast == nullptr && concat_this_node_maybe->parent->southWest->northEast == nullptr)
        {
            int amtElementsNE = concat_this_node_maybe->parent->northEast->children.size();
            int amtElementsNW = concat_this_node_maybe->parent->northWest->children.size();
            int amtElementsSE = concat_this_node_maybe->parent->southEast->children.size();
            int amtElementsSW = concat_this_node_maybe->parent->southWest->children.size();

            unsigned int sumElements = amtElementsNE + amtElementsNW + amtElementsSE + amtElementsSW;

            // move all elements from the leaf nodes into their parents node and delete the leaf nodes
            if (sumElements <= maxAmtElements)
            {
                // move elements from the northEast node to the parent node
                for (int i = 0; i < amtElementsNE; i++)
                {
                    float re_shuffle_x = concat_this_node_maybe->parent->northEast->children[i].x;
                    float re_shuffle_y = concat_this_node_maybe->parent->northEast->children[i].y;

                    pt2d reinsertPt(re_shuffle_x, re_shuffle_y);
                    concat_this_node_maybe->parent->children.push_back(reinsertPt);
                }

                // move elements from the northWest node to the parent node
                for (int i = 0; i < amtElementsNW; i++)
                {
                    float re_shuffle_x = concat_this_node_maybe->parent->northWest->children[i].x;
                    float re_shuffle_y = concat_this_node_maybe->parent->northWest->children[i].y;

                    pt2d reinsertPt(re_shuffle_x, re_shuffle_y);
                    concat_this_node_maybe->parent->children.push_back(reinsertPt);
                }

                // move elements from the southEast node to the parent node
                for (int i = 0; i < amtElementsSE; i++)
                {
                    float re_shuffle_x = concat_this_node_maybe->parent->southEast->children[i].x;
                    float re_shuffle_y = concat_this_node_maybe->parent->southEast->children[i].y;

                    pt2d reinsertPt(re_shuffle_x, re_shuffle_y);
                    concat_this_node_maybe->parent->children.push_back(reinsertPt);
                }

                // move elements from the southWest node to the parent node
                for (int i = 0; i < amtElementsSW; i++)
                {
                    float re_shuffle_x = concat_this_node_maybe->parent->southWest->children[i].x;
                    float re_shuffle_y = concat_this_node_maybe->parent->southWest->children[i].y;

                    pt2d reinsertPt(re_shuffle_x, re_shuffle_y);
                    concat_this_node_maybe->parent->children.push_back(reinsertPt);
                }

                // generate a pointer to the next node to concatenate (prevents an invalid read)
                Quadtree *concat_next = concat_this_node_maybe->parent;

                // delete the sibling nodes (of the removed point)
                concat_this_node_maybe->parent->clearNode();

                // proceed with the recursion
                concatenate_nodes(concat_next);
            }
        }
    }
}


[[maybe_unused]] bool Quadtree::relocate_element(pt2d ptOrigin, pt2d PtMoveTo)
{
    if (ptOrigin.x == PtMoveTo.x and ptOrigin.y == PtMoveTo.y)
    {
        return false;
    }

    Quadtree *nodePtReside_Origin = fetch_node(ptOrigin);

    // PtMoveTo lies outside of the node -> remove and reinsert this element
    if (PtMoveTo.x > nodePtReside_Origin->boundary2->cx+nodePtReside_Origin->boundary2->dim or PtMoveTo.x <= nodePtReside_Origin->boundary2->cx-nodePtReside_Origin->boundary2->dim or PtMoveTo.y > nodePtReside_Origin->boundary2->cy+nodePtReside_Origin->boundary2->dim or PtMoveTo.y <= nodePtReside_Origin->boundary2->cy-nodePtReside_Origin->boundary2->dim)
    {
        // TODO - remove element, reinsert into the parent node not the root node
        delete_element(ptOrigin);

        bool check_insert = insert(PtMoveTo);

        if (check_insert)
        {
            return true;
        }
            // element could not be fit into the root node, i.e., exited the root tree
        else
        {
            return false;
        }

    }
        //overwrite the point since it did not move out of the node
    else
    {
        // find the position of ptOrigin and overwrite its coordinates with the ones of PtMoveTo
        int foundIndex = -1;

        for (int i = 0; i < (int)nodePtReside_Origin->children.size(); i++)
        {
            if (nodePtReside_Origin->children[i].x == ptOrigin.x and nodePtReside_Origin->children[i].y == ptOrigin.y)
            {
                foundIndex = i;
                break;
            }
        }

        // update the coordinates, i.e., move the element
        nodePtReside_Origin->children[foundIndex].x = PtMoveTo.x;
        nodePtReside_Origin->children[foundIndex].y = PtMoveTo.y;
    }

    return true;
}

[[maybe_unused]] void Quadtree::removeChildren() {

}

std::vector<pt2d> create_random_point(){
    std::random_device rd;
    std::mt19937 gen(rd());

    std::vector<pt2d> point;

    uniform_int_distribution<int> intDistribution_1(1,4);
    uniform_int_distribution<int> intDistribution_2(6,9);


    //Region 1
    pt2d point1[5000];

    int dist_x_region1 = intDistribution_2(gen);
    float  dist_x_f_region1 = (float) dist_x_region1/10;

    int dist_y_region1 = intDistribution_2(gen);
    float  dist_y_f_region1 = (float) dist_y_region1/10;

    std::bernoulli_distribution bernoulli_x_region1(dist_x_f_region1);
    std::bernoulli_distribution bernoulli_y_region1(dist_y_f_region1);

    static double total_x_region1 = 0.0;
    static double total_y_region1 = 0.0;

    for (auto & i : point1) {
        for (int j=0;j<2048;++j) {
            total_x_region1 += bernoulli_x_region1(gen)*2048;
            total_y_region1 += bernoulli_y_region1(gen)*2048;
        }
        i.x = (float)total_x_region1 / 2048;
        i.y = (float)total_y_region1 / 2048;
        point.push_back(i);
        total_x_region1 = 0.0;
        total_y_region1 = 0.0;
    }

    //Region 2
    pt2d point2[5000];

    int dist_x_region2 = intDistribution_1(gen);
    float  dist_x_f_region2 = (float) dist_x_region2/10;

    int dist_y_region2 = intDistribution_2(gen);
    float  dist_y_f_region2 = (float) dist_y_region2/10;

    std::bernoulli_distribution bernoulli_x_region2(dist_x_f_region2);
    std::bernoulli_distribution bernoulli_y_region2(dist_y_f_region2);

    static double total_x_region2 = 0.0;
    static double total_y_region2 = 0.0;

    for (auto & i : point2) {
        for (int j=0;j<2048;++j) {
            total_x_region2 += bernoulli_x_region2(gen)*2048;
            total_y_region2 += bernoulli_y_region2(gen)*2048;
        }
        i.x = (float)total_x_region2 / 2048;
        i.y = (float)total_y_region2 / 2048;
        point.push_back(i);
        total_x_region2 = 0.0;
        total_y_region2 = 0.0;
    }

    //Region 3
    pt2d point3[5000];

    int dist_x_region3 = intDistribution_1(gen);
    float  dist_x_f_region3 = (float) dist_x_region3/10;

    int dist_y_region3 = intDistribution_1(gen);
    float  dist_y_f_region3 = (float) dist_y_region3/10;

    std::bernoulli_distribution bernoulli_x_region3(dist_x_f_region3);
    std::bernoulli_distribution bernoulli_y_region3(dist_y_f_region3);

    static double total_x_region3 = 0.0;
    static double total_y_region3 = 0.0;

    for (auto & i : point3) {
        for (int j=0;j<2048;++j) {
            total_x_region3 += bernoulli_x_region3(gen)*2048;
            total_y_region3 += bernoulli_y_region3(gen)*2048;
        }
        i.x = (float)total_x_region3 / 2048;
        i.y = (float)total_y_region3 / 2048;
        point.push_back(i);
        total_x_region3 = 0.0;
        total_y_region3 = 0.0;
    }


    //Region 4
    pt2d point4[5000];

    int dist_x_region4 = intDistribution_2(gen);
    float  dist_x_f_region4 = (float) dist_x_region4/10;

    int dist_y_region4 = intDistribution_1(gen);
    float  dist_y_f_region4 = (float) dist_y_region4/10;

    std::bernoulli_distribution bernoulli_x_region4(dist_x_f_region4);
    std::bernoulli_distribution bernoulli_y_region4(dist_y_f_region4);

    static double total_x_region4 = 0.0;
    static double total_y_region4 = 0.0;

    for (auto & i : point4) {
        for (int j=0;j<2048;++j) {
            total_x_region4 += bernoulli_x_region4(gen)*2048;
            total_y_region4 += bernoulli_y_region4(gen)*2048;
        }
        i.x = (float)total_x_region4 / 2048;
        i.y = (float)total_y_region4 / 2048;
        point.push_back(i);
        total_x_region4 = 0.0;
        total_y_region4 = 0.0;
    }

    pt2d point5[100];

    uniform_int_distribution<int> distribution_x(0,2048);
    uniform_int_distribution<int> distribution_y(0,2048);

    for (auto &vec : point5) {
        vec.x = distribution_x(gen);
        vec.y = distribution_y(gen);
        point.push_back(vec);
    }

    return  point;
}

void write_file(std::vector<pt2d> &pt) {
    std::ofstream output_file("point.docx");
    for (auto & i : pt) {
        output_file << i.x << " " << i.y << std::endl;
    }
}

void display(){
    std::vector<pt2d> point = create_random_point();
    write_file(point);

    float BB_center_x = .0;
    float BB_center_y = .0;
    float BB_dim = pow(2,11);

    std::shared_ptr<BoundaryBox> BB_init2(new BoundaryBox(BB_center_x, BB_center_y, BB_dim));

    // initialize the QT
    auto *quadtreeTest = new Quadtree(std::move(BB_init2), nullptr, 0);

    //insert a point
    pt2d *insertPt2[point.size()];

    for (int i = 0; i < point.size(); ++i) {
        insertPt2[i] = new pt2d(point[i].x,point[i].y);
        quadtreeTest->insert(*insertPt2[i]);
    }

    int tree_count_elements = quadtreeTest->count_elements(quadtreeTest);

    std::cout << "number of elements in tree:" << tree_count_elements << "\n";


    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); // NOLINT(hicpp-signed-bitwise)

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    glLineWidth(2.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(1.0, 0.0, 0.0);
    glVertex2f(-.5,0.5);
    glVertex2f(.5,0.5);
    glEnd();
    glLineWidth(2.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(1.0, 0.0, 0.0);
    glVertex2f(.5,.5);
    glVertex2f(.5,-.5);
    glEnd();

    glPointSize(4.0f);
    glBegin(GL_POINTS);

    double pos_x;
    double pos_y;

    for (auto & i : point) {
        pos_x = (double) i.x/102.4*.05;
        pos_y = (double) i.y/102.4*.05;
        glColor3f(0.0, 0.0, 1.0);
        glVertex2f(pos_x-.5,pos_y-.5);
    }

    glEnd();

    quadtreeTest->traverse_and_draw(quadtreeTest,BB_dim);


    double x_pos_1 = -.51;
    double y_pos_1 = -.55;

    const char *a[5] = {"0","512","1024","1536","2048"};

    for (auto & i : a) {
        glColor3f (.0, 1.0, .0);
        glRasterPos2f(x_pos_1, y_pos_1); //define position on the screen
        x_pos_1 += .25;

        while (*i){
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *i++);
        }
    }

    glFlush();
    glutSwapBuffers();

}

int main(int argc, char* argv[]){
    //  Initialize GLUT and process user parameters
    glutInit(&argc,argv);

    //  Request double buffered true color window with Z-buffer
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); // NOLINT(hicpp-signed-bitwise)

    // Create window
    glutCreateWindow("Coordinate System");

    //  Enable Z-buffer depth test
    glEnable(GL_DEPTH_TEST);

    // Callback functions
    glutDisplayFunc(display);

    //  Pass control to GLUT for events
    glutMainLoop();

    return 0;
}
