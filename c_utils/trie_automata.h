#include <cstdlib>
#include <vector>
#include <string>
#include <memory>
#include <queue>
#include <stack>
/**
 * interface
 * */

namespace trie_automata {


const unsigned int LOWER_MASK = 0xFF;
const unsigned int UPPER_MASK = 0xFF << 21;


const size_t ROOT_INDEX = 0;

class TextDict {
public:
    TextDict() {
    }

    void AddLine(const std::string& line) {
        size_t table_pos = line.find('\t');
        if (table_pos != std::string::npos) {
            keys_.push_back(line.substr(0, table_pos));
            values_.push_back(atoi(line.c_str() + table_pos + 1));
        } else {
            keys_.push_back(line);
            values_.push_back(line.size());
        }
    }

    bool Check() const {
    }

    void FindChildren(size_t begin, size_t end, size_t depth,
            std::vector<size_t>& begins, std::vector<size_t>& ends, 
            std::vector<unsigned char>& keys) const;

    size_t size() const {
        return keys_.size();
    }

    unsigned char key_at(size_t idx, size_t i) const {
        return keys_[idx][i];
    }
    int value(size_t idx) const {
        return values_[idx];
    }

private:
    std::vector<std::string> keys_;
    std::vector<int> values_;
};


template <class Node>
void save_trie(std::vector<Node>& trie, std::FILE* pf) {
    if (trie.size()) {
        fwrite(&trie[0], sizeof(Node), trie.size(), pf);
    }
}

template <class Node>
void load_trie(std::vector<Node>& trie, std::FILE* pf) {
    rewind(pf);
    fseek(pf, 0, SEEK_END);
    size_t file_size = ftell(pf);
    rewind(pf);
    trie.clear();
    size_t trie_size = file_size / sizeof(Node);
    trie.resize(trie_size);
    size_t rt = fread(&trie[0], sizeof(Node), trie_size, pf);
}

template <class Node>
void report_trie(std::vector<Node>& trie) {
    size_t free = 0;
    for (size_t i = 0; i < trie.size(); i++){
        trie[i].Dump();
        if (trie[i].IsFree()) {
            free++;
        }
    }
    printf("%lu %lu\n", trie.size(), free);

};


/**
 * 32bit
 * new_base = offset ^ node_pos
 * noe_pos = offset ^ pos ^ key
 * */
class TrieNode {
public :
    /** 返回对应的节点的8位编码
     * */
    unsigned int label() const {
        return trie_info_ & 0xC00000FF;
    }
    /** 返回用于生成新base的offset
     * */
    unsigned int offset() const {
        return (trie_info_ >> 10) << ((trie_info_ & (1U << 9)) >> 6);
    }
    /** 是否是叶子节点
     * */
    bool has_leaf() const {
        return ((trie_info_ >> 8) & 1) == 1;
    }

    int value() const {
        return trie_info_ ^ (1u << 31);
    }
    bool IsFree() const {
        return trie_info_ == 0;
    }

    void set_has_leaf() {
        trie_info_ |= 0x100;
    }
    void set_label(unsigned char label) {
        trie_info_ = (trie_info_ & 0xFFFFFF00) | label;
    }
    void set_value(int value) {
        trie_info_ = 0x80000000 | (value & 0x7FFFFFFF);
    }
    void set_offset(unsigned int offset) {
        if (offset >= 1U << 29) {
            throw std::exception();
        }
        trie_info_ &= (1U << 31) | (1U << 8) | 0xFF;
        if (offset < 1U << 21) {
            trie_info_ |= (offset << 10);
        } else {
            trie_info_ |= (offset << 2) | (1U << 9);
        }
    }
    TrieNode() {
        trie_info_ = 0;
    }
    void Dump() {
        printf("%u\n", trie_info_);
    }
protected:
    unsigned int trie_info_;
};


class ACNode : public TrieNode {
public:
    ACNode() {
        fail_ = ROOT_INDEX;
    }
    void set_fail(unsigned int fail) {
        fail_ = fail;
    }
    unsigned int fail() {
        return fail_;
    }
protected:
    unsigned int fail_;
};



void load_binary_file(const char* filename, char*& ptr, size_t& size) {
}



template <class Node>
class TrieAutomata {
public:
    TrieAutomata(std::vector<Node>& trie) :
        array_(&trie[0]), 
        size_(trie.size())
        {}
    TrieAutomata(const char* filename) {

    }
    unsigned int search_next(const unsigned int index, 
            const unsigned char label) const {
        auto cur = index;
        while (true) {
            auto next = cur ^ array_[cur].offset() ^ label;
            if (array_[next].label() == label) {
                return next;
            }
            if (cur == ROOT_INDEX) return ROOT_INDEX;
            cur = array_[cur].fail();
        }
    }

    void get_matches(const unsigned int index, 
            std::vector<int>& matches) const {

        matches.clear();
        auto cur = index;
        while (true) {
            if (cur == ROOT_INDEX) return;
            if (array_[cur].has_leaf()) {
                matches.push_back(
                        array_[cur ^ array_[cur].offset()].value());
            }
            cur = array_[cur].fail();
        }
    }
    
    int match(const char* key) const{
        unsigned int node_pos = 0;
        while (*key) {
            node_pos ^= array_[node_pos].offset();
            node_pos ^= *(unsigned char*)key;
            if (array_[node_pos].label() != *(unsigned char*)key) {
                return -1;
            }
            key++;
        }
        if (false == array_[node_pos].has_leaf()) {
            return -1;
        }
        node_pos ^= array_[node_pos].offset();
        return array_[node_pos].value();
    }
private:
    unsigned int size_;
    Node* array_;
};


/**implementations*/
void TextDict::FindChildren(size_t begin, size_t end, size_t depth,
        std::vector<size_t>& begins, std::vector<size_t>& ends, 
        std::vector<unsigned char>& keys) const {
    begins.clear();
    ends.clear();
    keys.clear();
    if (end == begin) return;
    size_t last_begin = begin;
    unsigned char last_key = key_at(begin, depth);
    for (size_t i = begin + 1; i < end; i++) {
        unsigned char k = key_at(i, depth);
        if (last_key != k) {
            begins.push_back(last_begin);
            ends.push_back(i);
            keys.push_back(last_key);
            last_begin = i;
            last_key = k;
        }
    }
    if (last_begin != end) {
        begins.push_back(last_begin);
        ends.push_back(end);
        keys.push_back(key_at(last_begin, depth));
    }
}

class FreeLink {
public:
    FreeLink(){
        free_info_.push_back(FreeInfo(0, 0));
        free_info_.front().used = true;
        head_ = 1;
        for (int i = 0; i < 8; i++) {
            Expand();
        }
    }

    void report() {
        size_t free = 0;
        for (size_t i = 0; i< free_info_.size(); i++) {
            printf("freelist %lu %u\n", i, IsFree(i));
            if (IsFree(i)) {
                free++;
            }
        }
        printf("free link %lu %lu\n", free_info_.size(), free);
    }

    unsigned int FindValid(unsigned int id,
            std::vector<unsigned char>& keys) {
        while (true) {
            unsigned int test_id = head_;
            if (!IsFull()) do {
                unsigned int offset = test_id ^ keys[0];
                if (IsValid(id, offset, keys)) {
                    //printf("free find %lu %lu\n", head_, offset);
                    SetUsed(id, offset, keys);
                    return offset;
                }
                test_id = free_info_[test_id].next;
            } while (test_id != head_);
            Expand();
        }
    };
private:

    bool IsFree(unsigned int idx) const {
        return !free_info_[idx].used;
    }
    void SetUsed(unsigned int idx) {
        free_info_[idx].used = true;
        unsigned int prev = free_info_[idx].prev;
        unsigned int next = free_info_[idx].next;
        free_info_[prev].next = next;
        free_info_[next].prev = prev;
        if (head_ == idx) { /// 使用了head
            if (head_ == next) { /// head本来是唯一free的节点
                head_ = free_info_.size();
            } else { /// head右移
                head_ = next;
            }
        }
    }
    bool IsValid(unsigned int id, unsigned int offset,
            std::vector<unsigned char>& keys) {
        unsigned int related_offset = offset ^ id;
        if ((related_offset & LOWER_MASK) && (related_offset & UPPER_MASK)) {
            return false;
        }
        for (auto key : keys) {
            if (!IsFree(key ^ offset)) {
                return false;
            }
        }
        return true;
    }
    void SetUsed(unsigned int id, unsigned int offset,
            std::vector<unsigned char>& keys) {
        for (auto key : keys) {
            SetUsed(key ^ offset);
        }
    }
    void Expand() {
        bool was_full = IsFull();
        size_t old_size = free_info_.size();
        for (size_t i = 0; i < old_size; i++) {
            free_info_.push_back(FreeInfo(free_info_.size() - 1, free_info_.size() + 1));
        }

        if (was_full) { 
            head_ = old_size;
        } else {
            size_t old_tail = free_info_[head_].prev;
            free_info_[old_tail].next = old_size;
            free_info_[old_size].prev = old_tail;
        }

        free_info_[head_].prev = free_info_.size() - 1;
        free_info_.back().next = head_;
    }
    struct FreeInfo {
        unsigned int prev;
        unsigned int next;
        bool used;
        FreeInfo(unsigned int p, unsigned int n) : 
            used(false), prev(p), next(n) {}
    };
    bool IsFull() const {
        return head_ == free_info_.size();
    }

    unsigned int head_;
    std::vector<FreeInfo> free_info_;
};

template <class Node>
class TrieBuilder {
public:
    TrieBuilder() {
        trie_ = std::make_shared<std::vector<Node>>();
        trie_->push_back(Node());
    }

    std::shared_ptr<std::vector<Node>> Build(TextDict& text_dict);
    std::shared_ptr<std::vector<Node>> nodes();
    void CalcFail(unsigned int this_node, 
        unsigned int parents_fail, unsigned char label); 

private:
    Node& node(unsigned int index) {
        while (index >= trie_->size()) {
            Expand();
        }
        return (*trie_)[index];
    }
    void Expand() {
        size_t old_size = trie_->size();
        for (size_t i = 0; i < old_size; i++) {
            trie_->push_back(Node());
        }
    }
    void Shrink() {
        auto new_trie = std::make_shared<std::vector<Node>>();
        size_t end = trie_->size();
        while (true) {
            if (end - 1 <= 1) break;
            if (!(*trie_)[end - 1].IsFree()) {
                break;
            }
            end--;
        }
        for (size_t i = 0; i < end; i++) {
            new_trie->push_back((*trie_)[i]);
        }
        trie_ = new_trie;
    }

    unsigned int ACRead(unsigned int index, unsigned char label) {
        while (true) {
            auto ch = index ^ node(index).offset() ^ label;
            if (node(ch).label() == label) {
                return ch;
            }
            if (index == ROOT_INDEX) {
                return ROOT_INDEX;
            }
            index = node(index).fail();
        }
    }

private:
    /// use for depth-first and width-first search
    struct SearchState {
        size_t begin;
        size_t end;
        size_t depth;
        size_t node_pos;
        SearchState(size_t b, size_t e,
                size_t d, size_t p) :
            begin(b), end(e), depth(d), node_pos(p){}
    };

    std::shared_ptr<std::vector<Node>> trie_;
    FreeLink free_link_;
};



template <class Node>
std::shared_ptr<std::vector<Node>> TrieBuilder<Node>::Build(TextDict& text_dict) {
    std::vector<size_t> begins;
    std::vector<size_t> ends;
    std::vector<unsigned char> keys;

    std::queue<std::shared_ptr<SearchState>> queue;
    std::vector<bool> node_used;

    queue.push(std::make_shared<SearchState>(
                0, text_dict.size(), 0, ROOT_INDEX
                ));

    
    /// 用队列实现搜索
    while (queue.size()) {
        /// 得到队列第一个元素
        auto state = queue.front();
        queue.pop();

        /// 得到当前节点的所有儿子
        text_dict.FindChildren(
                state->begin, 
                state->end, state->depth,
                begins, ends, keys);

        /// 分配TRIE树空间，填充内容
        unsigned int offset = free_link_.FindValid(state->node_pos, keys);

        node(state->node_pos).set_offset(state->node_pos ^ offset);

        /// 将非叶子节点的每个孩子也放入队列
        for (size_t i = 0; i < keys.size(); i++) {
            if (keys[i] == '\0') {
                node(state->node_pos).set_has_leaf();
                node(offset ^ keys[i]).set_value(text_dict.value(begins[i]));
                continue;
            }
            node(offset ^ keys[i]).set_label(keys[i]);

            CalcFail(offset ^ keys[i], state->node_pos, keys[i]);

            queue.push(std::make_shared<SearchState>(
                        begins[i], ends[i], state->depth + 1,
                        offset ^ keys[i]
                        ));
        }
    }
    Shrink();
    return trie_;
}

template <>
void TrieBuilder<TrieNode>::CalcFail(unsigned int this_node, 
        unsigned int parent, unsigned char label) {
}
template <>
void TrieBuilder<ACNode>::CalcFail(unsigned int this_node, 
        unsigned int parent, unsigned char label) {
    if (parent == ROOT_INDEX) {
        node(this_node).set_fail(ROOT_INDEX);
        return;
    }
    auto fail = ACRead(node(parent).fail(), label);
    //printf("%u(par: %u)'s fail is %u for label %lu\n", this_node, parent, fail, label);
    node(this_node).set_fail(fail);
}

}
