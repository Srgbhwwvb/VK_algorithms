#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <queue>
#include<bitset>
#include<string>
#include<math.h>
#include<typeinfo>

using namespace std;

// Node structure for the Huffman tree
struct Node {
    char symbol;
    int frequency;
    Node* left;
    Node* right;
};

// Comparison function for nodes in the priority queue
struct NodeCompare {
    bool operator()(Node* a, Node* b) {
        return a->frequency > b->frequency;
    }
};

// MinHeap struct
struct MinHeap {
    vector<Node*> heap;
    void insert(Node* node) {
        heap.push_back(node);
        int i = heap.size() - 1;
        while (i > 0) {
            int parent = (i - 1) / 2;
            if (heap[parent]->frequency <= heap[i]->frequency) break;
            swap(heap[parent], heap[i]);
            i = parent;
        }
    }
    Node* extractMin() {
        Node* min = heap[0];
        heap[0] = heap.back();
        heap.pop_back();
        int i = 0;
        while (true) {
            int left = 2 * i + 1;
            int right = 2 * i + 2;
            int smallest = i;
            if (left < heap.size() && heap[left]->frequency < heap[smallest]->frequency) {
                smallest = left;
            }
            if (right < heap.size() && heap[right]->frequency < heap[smallest]->frequency) {
                smallest = right;
            }
            if (smallest == i) break;
            swap(heap[i], heap[smallest]);
            i = smallest;
        }
        return min;
    }
};

class BitWriter
{
public:
    BitWriter() :bitCount(0) {}
    void WriteBit(unsigned char bit)
    {
        if (bitCount % 8 == 0)
        {
            buffer.push_back(0);
        }
        if (bit)
        {
            buffer[bitCount / 8] |= 1 << (7 - bitCount % 8);
            bitCount++;

        }
        if (!bit)
        {
            buffer[bitCount / 8] |= 0 << (7 - bitCount % 8);
            bitCount++;
        }
    }

    void WriteByte(unsigned char byte)
    {
        if (bitCount % 8 == 0)
        {
            buffer.push_back(byte);
        }
        else
        {
            int offset = bitCount % 8;
            buffer[bitCount / 8] |= byte >> offset;
            buffer.push_back(byte << (8 - offset));
        }
        bitCount += 8;
    }
    const vector<unsigned char>& getBuffer() const
    {
        return buffer;
    }
    size_t getBitCount() const
    {
        return bitCount;
    }

private:
    vector<unsigned char> buffer;
    size_t bitCount;
};
void vizualizeBuffer(const vector<unsigned char>& buffer)
{
    for (auto& b : buffer)
    {
        cout << bitset<8>(b) << "|";
    }
    cout << endl;

}
// Huffman tree class
class BitReader {
public:

    void charToBinary(char byte) {
        vector<unsigned char> binary;
        bitset<8> bits(byte); // 8 bits for a char

        for (int i = 7; i >= 0; --i) {
            if (bitCount % 8 == 0)
            {
                buffer.push_back(0);
            }
            if (bits[i]==1)
            {
                buffer[bitCount / 8] |= 1 << (7 - bitCount % 8);
                bitCount++;

            }
            if (bits[i]==0)
            {
                buffer[bitCount / 8] |= 0 << (7 - bitCount % 8);
                bitCount++;
            }
        }
        
    }




    BitReader(const vector<unsigned char>& buffer_) : buffer(buffer_), bitCount(0) {}
    BitReader(ifstream& input)
    {
        bitCount = 0;
        char byte;
        while (input.get(byte)) {
            charToBinary(byte);//запись в буффер
            
           
        }
        bitCount = 0;
    }
 
    unsigned char ReadBit() {
        unsigned char bit = (buffer[bitCount / 8] >> (7-bitCount % 8)) & 1;
        bitCount++;
        
        return bit;
    }

    /*unsigned char ReadByte() {
        unsigned char byte = 0;
        if (bitCount % 8 == 0) {
            byte = buffer[bitCount / 8];
            bitCount += 8;
        }
        else {
            int offset = bitCount % 8;
            byte = (buffer[bitCount / 8] << offset) | (buffer[bitCount / 8 + 1] >> (8 - offset));
            bitCount += 8;
        }
        return byte;
    }*/
    unsigned char ReadByte() {    
        int result = 0;
        if (bitCount % 8 == 0)
        {
            unsigned char a = buffer[bitCount / 8];
            bitCount += 8;
            return a;
        }
        else
        {
            vector<unsigned char> res(8);
            for (int i = 0;i < 8;i++)
            {
                res[i]= (buffer[bitCount / 8] >> (7 - bitCount % 8)) & 1;
                bitCount++;
                if (res[i] == 1)
                {
                    result += pow(2, 7 - i);
                }
            }
            return static_cast<char>(result);
        }

        unsigned char a = buffer[bitCount];
        bitCount+=8;
        return a;
    }
    vector<unsigned char> ReadBytes(size_t count) {
        vector<unsigned char> bytes;
        for (size_t i = 0; i < count; i++) {
            bytes.push_back(ReadByte());
        }
        return bytes;
    }

    size_t getBitCount() const {
        return bitCount;
    }
    const vector<unsigned char>& getBuffer() const
    {
        return buffer;
    }

private:
    vector<unsigned char> buffer;
    size_t bitCount;
};
class HuffmanTree {
public:
    int alphabet = 0;
    Node* root;
    int useful_bits = 0;
    HuffmanTree() : root(nullptr) {}
    ~HuffmanTree() {
        deleteTree(root);
    }
    void deleteTree(Node* node) {
        if (node) {
            deleteTree(node->left);
            deleteTree(node->right);
            delete node;
        }
    }
private:

public:
    void levelOrder() {
        if (root == NULL) return;

        queue<Node*> q;
        q.push(root);

        while (!q.empty()) {
            Node* temp = q.front();
            q.pop();

            cout << temp->frequency << " ";

            if (temp->left != NULL) q.push(temp->left);
            if (temp->right != NULL) q.push(temp->right);
        }
    }
    void buildTree(const string& input) {
        MinHeap heap;
        vector<int> frequency(256, 0);
        for (char c : input) {
            frequency[c]++;
        }
        for (int i = 0;i < 256;i++)
        {
            if (frequency[i] != 0)
            {
                alphabet++;
            }
        }
        for (int i = 0; i < 256; i++) {
            if (frequency[i] > 0) {
                Node* node = new Node();
                node->symbol = i;
                node->frequency = frequency[i];
                node->left = node->right = nullptr;
                heap.insert(node);
            }
        }
        while (heap.heap.size() > 1) {
            Node* left = heap.extractMin();
            Node* right = heap.extractMin();
            Node* parent = new Node();
            parent->frequency = left->frequency + right->frequency;
            parent->left = left;
            parent->right = right;
            heap.insert(parent);
        }
        root = heap.extractMin();
        useful_bits = countEncodedBits(input) % 9;

    }

public:

    void writeAlphabet(BitWriter& writer, int alphabet)
    {
        writer.WriteByte(static_cast<char> (alphabet));
    }
    void writeBits(BitWriter& writer, int bits)
    {
        writer.WriteByte(static_cast<char>(bits));
    }
    void writeTreetoBuffer(BitWriter& writer, Node* node) {
        if (node->left == nullptr && node->right == nullptr) {
            writer.WriteBit(1); // mark as leaf node
            writer.WriteByte(node->symbol); // write alphabet symbol
        }
        else {
            writeTreetoBuffer(writer, node->left); // recursively write left subtree
            writeTreetoBuffer(writer, node->right);
            writer.WriteBit(0);
        }
       
        


    }
    void writeTree(BitWriter& writer, ofstream& output, string& input)
    {
        writeAlphabet(writer, alphabet);
        writeTreetoBuffer(writer, root);
        writeBits(writer, useful_bits);
        encodeData(writer, input);
        int n = writer.getBitCount();
        if (n % 8 == 0)
        {
            n = n / 8;
        }
        else
        {
            n = n / 8 + 1;
        }
        for (int i = 0; i < n; i++)
        {
             output.put(writer.getBuffer()[i]);
        }

    }

    void encodeData(BitWriter& writer, string& input) {
        Node* node = root;
        int bitCount = 0;
        string path;
        for (char c : input) {
            path = getCode(c);

            for (int i = 0;i < path.size();i++)
            {

                if (path[i] == '1') {
                    writer.WriteBit(1);
                }
                else
                {
                    writer.WriteBit(0);
                }
            }
        }
    }

    void readTree(BitReader& reader, ifstream& input, string& output, ofstream& outfile) {
        int alphabetSize = reader.ReadByte();
        Node* root = readTreeFromBuffer(reader, alphabetSize);
        int usefulBits = reader.ReadByte();

        string res = decodeData(reader, output, root, usefulBits);
        outfile << res;
    }
    Node* readTreeFromBuffer(BitReader& reader, int alphabetSize) {
        stack<Node*> nodeStack;
        int count = 0;
        while(count!=alphabetSize || nodeStack.size()>1) {
            int bit = reader.ReadBit();
            if (bit == 1) {
                char symbol = reader.ReadByte();
                Node* node = new Node();
                node->symbol = symbol;
                node->left = nullptr;
                node->right = nullptr;
                nodeStack.push(node);
                count++;
            }
            else {
                Node* rightChild = nodeStack.top();
                nodeStack.pop();
                Node* leftChild = nodeStack.top();
                nodeStack.pop();
                Node* newNode = new Node();
                newNode->left = leftChild;
                newNode->right = rightChild;
                nodeStack.push(newNode);
            }
        }
        return nodeStack.top();
    }
    int num_bytes(BitReader& reader)
    {
        int n = reader.getBitCount();
        if (n%8==0)
        {
            return n / 8 ;
        }
        else
        {
            return n / 8 + 1;
        }
    }
    string decodeData(BitReader& reader, string& output, Node* root, int usefulBits) {
        Node* node = root;
        int count = 0;
        int n = 0;
        while (true) {
            n = num_bytes(reader);
            
            if (reader.getBuffer().size() == n )
            {
                count = count % 8;
                while (count < usefulBits)
                {
                    int bit = reader.ReadBit();
                    if (bit == 0) {
                        node = node->left;
                        count++;
                    }
                    else {
                        node = node->right;
                        count++;
                    }
                    if (node->left == nullptr && node->right == nullptr) {
                        output += node->symbol;
                        node = root;
                    }
                }
                break;
            }
            int bit = reader.ReadBit();
            if (bit == 0) {
                node = node->left;
                count++;
            }
            else {
                node = node->right;
                count++;
            }
            if (node->left == nullptr && node->right == nullptr) {
                output += node->symbol;
                node = root;               
            }
            
        }
        return output;
    }
    int countEncodedBits(string text) {
        int bits = 0;
        for (char c : text) {
            string code = getCode(c);
            bits += code.length();
        }
        return bits;
    }
    string getCode(char ch, string code = "") {
        stack<pair<Node*, string>> stack;
        Node* node = root;
        stack.push({ root, code });

        while (!stack.empty()) {
            Node* node = stack.top().first;
            string currCode = stack.top().second;
            stack.pop();

            if (node->right != nullptr) {
                stack.push({ node->right, currCode + "1" });
            }

            if (node->symbol == ch) {
                cout << node->symbol << endl;
                cout << currCode << endl;
                return currCode;
            }

            if (node->left != nullptr) {
                stack.push({ node->left, currCode + "0" });
            }
        }

        return "";
    }

};
int main() {
    string input = "poooooi";
    HuffmanTree tree;
    BitWriter bw;
    ofstream output("output.bin", ios::binary);
    tree.buildTree(input);//correct
    tree.levelOrder();
    tree.writeTree(bw,output, input);
    vizualizeBuffer(bw.getBuffer());

    output.close();


    ifstream input2("output.bin", ios::binary);
    BitReader br(input2);
    vizualizeBuffer(br.getBuffer());
    cout << "num bytes " << br.getBitCount() << endl;
    ofstream output2("decoded.txt");
    string a;
    tree.readTree(br, input2, a, output2);
    input2.close();
    output2.close();

    return 0;
}































