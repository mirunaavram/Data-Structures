//Surse: https://stackoverflow.com/questions/19697761/finding-successor-in-a-b-tree
//https://www.youtube.com/watch?v=kjBI0rimo-w am parcurs intregul video pentru a intelege implementarea https://github.com/mayowa-ops/btree-/blob/main/main.cpp
//https://www.tutorialspoint.com/cplusplus-program-to-perform-sorting-using-b-tree
//https://www.geeksforgeeks.org/insert-operation-in-b-tree/

///codul facut si inteles inainte - nu cred ca insereaza corect sau e problema la afisare
///functia de delete am incercat o lazy deletionl, celelalte functii sunt testate pe codul de inserare de pe GeeksForGeeks de mai jos
///si functioneaza din cate am observat

#include <bits/stdc++.h>
using namespace std;
ifstream f("abce.in");
ofstream g("abce.out");

unordered_map<int, int> umap; //pentru a marca nodurile sterse si le omit la afisare

struct nod{
    int i,numarChei;
    int* cheie;
    nod** copil; //pointeaza la nodul parinte
    int esteFrunza; //verific daca este frunza sau nu
    nod(int numar){ //initializeaza nodul
        numarChei=0;
        cheie = new int[numar];
        esteFrunza = 1;
        copil = new nod*[numar+1]; //poate avea n-1 chei si n copii -> splituiesc pentru cand e full cheia
        for(i=0;i<numar;i++)
            copil[i]= nullptr;
    }

    nod* gasestePozitie(int valoare,nod* radacina, int grad,nod* nod_constant);
    void splitCopil(nod* copil,int grad);
    //void fullCheck(nod* temp,int grad);
    //void afiseazaNivel(nod* temp,int nivel);
    //void inaltime(nod* temp,int inaltime,int& rezultat);
    int succesor(int val);
    int predecesor(int val);
    void traverse();
    void sortatIntre(int left,int right);
    int cauta(int k);
};

int nod::cauta(int k)
{
    // prima cheie>=k
    int i = 0;
    while (i < numarChei && k > cheie[i])
        i++;
    if (cheie[i] == k && umap[cheie[i]]==0)
        return 1;
    if (esteFrunza == 1)
        return 0;
    return copil[i]->cauta(k); //ma duc la urmatorul copil
}

nod* nod::gasestePozitie(int valoare,nod* radacina, int t, nod* nod_constant) {
    if (esteFrunza) {
        int i = t;
        while (!cheie[i - 1]) {
            i--; //ajung la valoarea pe care nu o cunosc
        }
        while (cheie[i - 1] > valoare && i != 0) {
            cheie[i] = cheie[i - 1]; //fac loc pentru valoarea noua
            i--;
        }
        cheie[i] = valoare;
        numarChei += 1;
    }
    else{ //daca nu este frunza ma duc la respectivul copil
        int i=0;
        while(i<numarChei && valoare>cheie[i]) {
            i++; //stiu la care copil sa ma duc
        }
        copil[i]->gasestePozitie(valoare,this,t,nod_constant);
    }
    //am gasit respectivul copil
    if(numarChei == t) //verific daca trebuie sa splituiesc cand atinge maximul de ocupare
    {
        //daca e radacina trebuie sa creez un nou nod
        if(this==nod_constant){
            nod* p=new nod(t);
            p->esteFrunza=0;
            p->copil[0]=this; //nu avea copii initial
            p->splitCopil(this,t); //p e parintele, iar this e copilul
            return p;
        }
            //daca nu e radacina
        else{
            radacina->splitCopil(this,t);
        }
    }
    return nod_constant;
}

void nod::splitCopil(nod *fullNod, int t) {
    nod*dreapta = new nod(t);
    int i_dreapta = 0, i;
    int urca = (t - 1) / 2;
    //obtin numarul de chei in nod
    int nod_numarChei = fullNod->numarChei;
    int carry = fullNod->cheie[urca]; //valoarea pe care o pun in parinte

    //daca nodul pe care-l splituiesc are copii
    int umbla_copil = ceil(float(t)/2);
    int i_copil = 0;

    for (i = urca + 1; i < nod_numarChei; i++) {
        dreapta->cheie[i_dreapta] = fullNod->cheie[i];
        i_dreapta++;
        fullNod->numarChei = fullNod->numarChei - 1;
        dreapta->numarChei += 1;
    }

    if (fullNod->esteFrunza == 0) { //daca are copii
        for (int index = umbla_copil; index <= t; index++) {
            dreapta->copil[i_copil] = fullNod->copil[index];
            i_copil++;
        }
        dreapta->esteFrunza = fullNod->esteFrunza;
    }
    //mut carry in varf
//creez spatiu in copil pentru a pune nodul drept
    int hold = t - 1;
    while (copil[hold] != fullNod) {
        copil[hold+1]=copil[hold];
        hold--;
    }
    copil[hold+1]=dreapta;
    int j=t-1;
    //*carry in pozitia dreapta cu parintele
    while(!cheie[j-1] && j!=0){
        //impingem in dreapta
        //cheie[j]=cheie[j-1];
        j--;
    }
    //daca valoarea inainte de cheie este mai mare decat valoarea punem in dreapta
    while(cheie[j-1]>carry && j!=0){
        cheie[j]=cheie[j-1];
        j--;
    }
    //inserez valoarea respectiva
    cheie[j]=carry;
    numarChei+=1;
    fullNod->numarChei=fullNod->numarChei-1;
}
void nod::traverse()
{
    //sunt n chei si n+1 copii, traverseaza n chei si primii n copii
    int i;
    for (i = 0; i < numarChei; i++)
    {
        // daca nu este frunza, traverseaza tot subarborele cu copilul[i]
        if (esteFrunza == 0)
            copil[i]->traverse();
        g<<" "<< cheie[i];
    }

    // Print the subtree rooted with last child
    if (esteFrunza == 0)
        copil[i]->traverse();
}


int nod::succesor(int val) {
    int i=0;
    while(i<numarChei && val>cheie[i]){
        i++;
    }
    if(i==numarChei){
        if(this->esteFrunza)
            return -1; //nu are succesor
            return copil[numarChei]->succesor(val); //nu e frunza, val> toate cheile
                                                    // caut in ultimul copil care are cheia>this->cheeie[numarChei-1]
    }
    //am gasit val, este propriul sau succesor
    if(val==cheie[i] || this->esteFrunza) //&umap[i]==0, dar nu-mi afiseaza nimic
        return cheie[i];
    //val<cheie[i] si nu e o frunza rezulta ca
    //succesorul e fie cheie[i] ori succesorul lui val in copil[i] daca exista
    int suc=copil[i]->succesor(val);
    return min(suc,cheie[i]);
}
int nod::predecesor(int val) {
    int i=numarChei-1;
    while(i>=0 && val<cheie[i]){
        i--;
    }
    if(i==-1){ //fiecare cheie>val
        if(this->esteFrunza) //nu are niciun predecesor
            return -1;
        return copil[0]->predecesor(val); //nu e frunza, val<cheie[0], dar si copil[0] are cheie<this->cheie[0]
    }
    if(val==cheie[i] || this->esteFrunza) //propriul sau predecesor //&umap[i]==0, dar nu-mi afiseaza nimic
        return cheie[i];
    //val>cheie[i] si nu e o frunza, rezulta ca predecesorul lui val e fie cheie[i]
    //sau predecesorul lui val in copil[i]
    //val<this->cheie[i+1]
    int pred= copil[i+1]->predecesor(val);
    //gasit in copilul i+1
    return max(pred,cheie[i]);
}
void nod::sortatIntre(int left, int right) {
    int i=0,index,j=numarChei-1;
    while(i<numarChei && cheie[i]<left)
        i++;
    while(j>=0 && cheie[j]>right)
        j--;
    if(this->esteFrunza){
        for(index=i;index<=j;index++)
        {
            if(umap[cheie[index]]==0)
                g<<cheie[index]<<" ";
        }
    }
    else{
        if(i==numarChei) //nicio cheie>=stanga
            copil[numarChei]->sortatIntre(left,right);
        else{
            if(j==-1)
                copil[0]->sortatIntre(left,right);

            else {
                if(i<=j) {
                    if (cheie[i] > left) //pot fi chei>=left in copilul[i]
                        copil[i]->sortatIntre(left, right);
                    for (index=i; index<j; index++) {
                        if(umap[cheie[index]]==0)
                            g << cheie[index] << " ";
                        copil[index + 1]->sortatIntre(left, right);
                    }
                    if(umap[cheie[j]]==0)
                        g << cheie[j] << " "; // pot fi chei<=right in copil[j+1]
                    if (cheie[j] < right)
                        copil[j + 1]->sortatIntre(left, right); //right= cheie[j] nu mai e o cheie mai mare de afisat
                }
            }
        }
    }

}



class BArbore{

private:
    nod* m_radacina;
    int m_t;
public:
    BArbore(){};

    BArbore(int val) {
        m_radacina= nullptr;
        m_t=val;
    }
    nod* getRdacina(){
        return m_radacina;
    }
    void insereaza(int valoare){
        if(m_radacina == nullptr){
            //putem insera
            m_radacina = new nod(m_t); //m_numar=grad
            m_radacina->esteFrunza=1;
            m_radacina->cheie[0] = valoare; //cum era gol, se insereaza pe prima pozitie
            //m_radacina->numarChei+=1;
            m_radacina->numarChei=1;
        }
        else{
            m_radacina=m_radacina->gasestePozitie(valoare,m_radacina,m_t,m_radacina); //gaseste pozitia corecta pentru a pune radacina
        }
    }
    ///pentru a verifica adaugarea datelor
    /*void afiseazaNivel(int nivel){
        m_radacina->afiseazaNivel(m_radacina,nivel);
    }
    int inaltime(){
        int rezultat;
        m_radacina->inaltime(m_radacina,1,rezultat);
        return rezultat;
    }*/
    //////////////////////////////////////////////
    void traverse()
    {
        if (m_radacina != nullptr)
            m_radacina->traverse();
    }
    void sortatIntre(int left,int right){
        m_radacina->sortatIntre(left,right);
        g<<endl;
    }
    int cauta(int k)
    {
        if(m_radacina != nullptr)
            return m_radacina->cauta(k);
        else
            return 0;
    }

    int predecesor(int numar) {
        return m_radacina->predecesor(numar);
    }
    int succesor(int val) {
        return m_radacina->succesor(val);
    }
};

int main() {
    int q,i,x,y,comanda;
    BArbore bArbore(3);
    f>>q;
    for(i=0;i<q;i++){
        f>>comanda;
        if(comanda==1){
            f>>x;
            bArbore.insereaza(x);
        }
        else if(comanda==2){
            f>>x;
            //lazy deletion
            umap[x]=1; ///marchez ce sterg
            //g<<"Se sterge "<<x<<'\n';
        }
        else if(comanda==3){
            f>>x;
            if(bArbore.cauta(x)!=0 && umap[x]==0)
                g<<"1"<<'\n';
            else g<<"0"<<'\n';
        }
        else if(comanda==4){
            f>>x;
            g<<bArbore.predecesor(x)<<'\n';
        }
        else if(comanda==5){
            f>>x;
            g<<bArbore.succesor(x)<<'\n';
        }
        else if(comanda==6){
            f>>x>>y;
            bArbore.sortatIntre(x,y);
            g<<'\n';
        }

    }

    return 0;
}
///am incercat sa vad daca imi baga datele corect
/*void nod::afiseazaNivel(nod *temp, int nivel) {
    if(nivel>1 &&esteFrunza){
        return;
    }
    else if(nivel==1){
        for(int i=0;i<numarChei;i++){
            g<<cheie[i]<<" ";
        }
    }
    else{
        for(i=0;i<=numarChei;i++){
            copil[i]->afiseazaNivel(temp,nivel-1);
        }
    }
}*/
/*void nod::inaltime(nod *temp, int inaltime, int &rezultat) {
    if(esteFrunza){
        rezultat=inaltime;
        return;
    }
    copil[0]->inaltime(temp,inaltime+1,rezultat);
}
*/
///
/*
#include<bits/stdc++.h>
using namespace std;

// A BTree node
class BTreeNode
{
    int *keys;  // An array of keys
    int t;      // Minimum degree (defines the range for number of keys)
    BTreeNode **C; // An array of child pointers
    int n;     // Current number of keys
    bool leaf; // Is true when node is leaf. Otherwise false
public:
    BTreeNode(int _t, bool _leaf);   // Constructor

    // A utility function to insert a new key in the subtree rooted with
    // this node. The assumption is, the node must be non-full when this
    // function is called
    void insertNonFull(int k);

    void splitChild(int i, BTreeNode *y);
    void traverse();

    int successor(int val);
    int predecessor(int val);
    void sortBetween(int left,int right);
///definition
    // A function to search a key in the subtree rooted with this node.
    BTreeNode *search(int k);   // returns nullptr if k is not present.

// Make BTree friend of this so that we can access private members of this
// class in BTree functions*/
 /*   friend class BTree;
};


// Constructor for BTreeNode class
BTreeNode::BTreeNode(int t1, bool leaf1)
{
    // Copy the given minimum degree and leaf property
    t = t1;
    leaf = leaf1;

    // Allocate memory for maximum number of possible keys
    // and child pointers
    keys = new int[2*t-1];
    C = new BTreeNode *[2*t];

    // Initialize the number of keys as 0
    n = 0;
}

// Function to traverse all nodes in a subtree rooted with this node
void BTreeNode::traverse()
{
    // There are n keys and n+1 children, traverse through n keys
    // and first n children
    int i;
    for (i = 0; i < n; i++)
    {
        // If this is not leaf, then before printing key[i],
        // traverse the subtree rooted with child C[i].
        if (leaf == false)
            C[i]->traverse();
        g << " " << keys[i];
    }

    // Print the subtree rooted with last child
    if (leaf == false)
        C[i]->traverse();
}

// Function to search key k in subtree rooted with this node
BTreeNode *BTreeNode::search(int k)
{
    // Find the first key greater than or equal to k
    int i = 0;
    while (i < n && k > keys[i])
        i++;

    // If the found key is equal to k, return this node
    if (keys[i] == k)
        return this;

    // If key is not found here and this is a leaf node
    if (leaf == true)
        return nullptr;

    // Go to the appropriate child
    return C[i]->search(k);
}


int BTreeNode::predecessor(int val) {
    int i=n-1;
    while(i>=0 && val<keys[i])
        i--;
    if(i==-1){
        if(this->leaf)
            return -1;
        return C[0]->predecessor(val);
    }
    if(val == keys[i] || this->leaf)
        return keys[i];
    int pred=C[i+1]->predecessor(val);
    return max(pred,keys[i]);
}

int BTreeNode::successor(int val){
    int i=0;
    while(i<n && val>keys[i])
        i++;
    if(i==n){
        if(this->leaf)
            return -1;
        return C[n]->successor(val);
    }
    if(val==keys[i] || this->leaf)
        return keys[i];
    int suc=C[i]->successor(val);
    return min(suc,keys[i]);
}

void BTreeNode::sortBetween(int left, int right) {
    int i=0,index;
    int j=n-1;
    while(i<n && keys[i]<left)
        i++;
    while(j>=0 && keys[j]>right)
        j--;
    if(this->leaf){
        for(index=i;index<=j;index++)
            g<<keys[index]<<" ";
    }
    else{
        if(i==n) //nicio cheie>=stanga
            C[n]->sortBetween(left,right);
        else{
            if(j==-1)
                C[0]->sortBetween(left,right);

            else {
                if(i<=j) {
                    if (keys[i] > left) //pot fi chei>=left in copilul[i]
                        C[i]->sortBetween(left, right);
                    for (index=i; index<j; index++) {
                        g << keys[index] << " ";
                        C[index + 1]->sortBetween(left, right);
                    }
                    g << keys[j] << " "; // pot fi chei<=right in C[j+1]
                    if (keys[j] < right)
                        C[j + 1]->sortBetween(left, right); //right= keys[j] nu mai e o cheie mai mare de afisat
                }
            }
        }
    }

}







// A BTree
class BTree
{
    BTreeNode *root; // Pointer to root node
    int t;  // Minimum degree
public:
    // Constructor (Initializes tree as empty)
    BTree(int _t)
    {  root = nullptr;  t = _t; }

    // function to traverse the tree
    void traverse()
    {  if (root != nullptr) root->traverse(); }

    // function to search a key in this tree
    BTreeNode* search(int k)
    {  return (root == nullptr)? nullptr : root->search(k); }

    // The main function that inserts a new key in this B-Tree
    void insert(int k);
    int successor(int val);
    int predecessor(int val);
    void sortBetween(int left,int right);
};


// The main function that inserts a new key in this B-Tree
void BTree::insert(int k)
{
    // If tree is empty
    if (root == nullptr)
    {
        // Allocate memory for root
        root = new BTreeNode(t, true);
        root->keys[0] = k;  // Insert key
        root->n = 1;  // Update number of keys in root
    }
    else // If tree is not empty
    {
        // If root is full, then tree grows in height
        if (root->n == 2*t-1)
        {
            // Allocate memory for new root
            BTreeNode *s = new BTreeNode(t, false);

            // Make old root as child of new root
            s->C[0] = root;

            // Split the old root and move 1 key to the new root
            s->splitChild(0, root);

            // New root has two children now.  Decide which of the
            // two children is going to have new key
            int i = 0;
            if (s->keys[0] < k)
                i++;
            s->C[i]->insertNonFull(k);

            // Change root
            root = s;
        }
        else  // If root is not full, call insertNonFull for root
            root->insertNonFull(k);
    }
}

// A utility function to insert a new key in this node
// The assumption is, the node must be non-full when this
// function is called
void BTreeNode::insertNonFull(int k)
{
    // Initialize index as index of rightmost element
    int i = n-1;

    // If this is a leaf node
    if (leaf == true)
    {
        // The following loop does two things
        // a) Finds the location of new key to be inserted
        // b) Moves all greater keys to one place ahead
        while (i >= 0 && keys[i] > k)
        {
            keys[i+1] = keys[i];
            i--;
        }

        // Insert the new key at found location
        keys[i+1] = k;
        n = n+1;
    }
    else // If this node is not leaf
    {
        // Find the child which is going to have the new key
        while (i >= 0 && keys[i] > k)
            i--;

        // See if the found child is full
        if (C[i+1]->n == 2*t-1)
        {
            // If the child is full, then split it
            splitChild(i+1, C[i+1]);

            // After split, the middle key of C[i] goes up and
            // C[i] is splitted into two.  See which of the two
            // is going to have the new key
            if (keys[i+1] < k)
                i++;
        }
        C[i+1]->insertNonFull(k);
    }
}

// A utility function to split the child y of this node
// Note that y must be full when this function is called
void BTreeNode::splitChild(int i, BTreeNode *y)
{
    // Create a new node which is going to store (t-1) keys
    // of y
    BTreeNode *z = new BTreeNode(y->t, y->leaf);
    z->n = t - 1;

    // Copy the last (t-1) keys of y to z
    for (int j = 0; j < t-1; j++)
        z->keys[j] = y->keys[j+t];

    // Copy the last t children of y to z
    if (y->leaf == false)
    {
        for (int j = 0; j < t; j++)
            z->C[j] = y->C[j+t];
    }

    // Reduce the number of keys in y
    y->n = t - 1;

    // Since this node is going to have a new child,
    // create space of new child
    for (int j = n; j >= i+1; j--)
        C[j+1] = C[j];

    // Link the new child to this node
    C[i+1] = z;

    // A key of y will move to this node. Find the location of
    // new key and move all greater keys one space ahead
    for (int j = n-1; j >= i; j--)
        keys[j+1] = keys[j];

    // Copy the middle key of y to this node
    keys[i] = y->keys[t-1];

    // Increment count of keys in this node
    n = n + 1;
}
int BTree::successor(int val) {
    return root->successor(val);
}
int BTree::predecessor(int val) {
    return root->predecessor(val);
}
void BTree::sortBetween(int left, int right) {
    root->sortBetween(left,right);
    g<<endl;
}


int main()
{
    BTree t(3); // A B-Tree with minium degree 3
    int q,i,op,x,y;
    cin>>q;
    for(i=0;i<q;i++)
    {
        cin>>op;
        if(op==1){
            cin>>x;
            t.insert(x);
        }
        else if(op==2){
            cin>>x;
        }
        else if(op==3){
            cin>>x;
            g<<t.search(x)<<endl;}
        else if(op==4){
            cin>>x;
            g<<t.predecessor(x)<<endl;
        }
        else if(op==5){
            cin>>x;
            g<<t.successor(x)<<endl;
        }
        else if(op==6){
            cin>>x>>y;
            t.sortBetween(x,y);
            g<<endl;
        }
    }
*/
    /*t.insert(10);
    t.insert(20);
    t.insert(5);
    t.insert(6);
    t.insert(12);
    t.insert(30);
    t.insert(7);
    t.insert(17);
    g<<t.successor(17)<<endl;
    g<<t.predecessor(17)<<endl;
    t.sortBetween(5,12);
    g << "Traversal of the constucted tree is ";
    t.traverse();

    int k = 6;
    (t.search(k) != nullptr)? g << "\nPresent" : g << "\nNot Present";

    k = 15;
    (t.search(k) != nullptr)? g << "\nPresent" : g << "\nNot Present";


    return 0;
}
     */
