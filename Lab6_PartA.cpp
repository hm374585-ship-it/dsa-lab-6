#include <iostream>
#include <string>
#include <utility>
#include <limits>
using namespace std;

// ========================================
// Book Node
// ========================================
struct BookNode
{
    int bookID;
    string title;
    BookNode* next;
};

// ========================================
// Circular Linked List Class
// head points to the last node (pivot)
// head->next points to the first node
// ========================================
class CircularLinkedList
{
private:
    BookNode* head;
    int count;
    int cursor;     // index of the next book to be shown on the display screen

    BookNode* GetNodeAt(int index)
    {
        if (head == nullptr)
            return nullptr;
        BookNode* current = head->next;
        for (int i = 0; i < index; i++)
            current = current->next;
        return current;
    }

    void SwapData(BookNode* a, BookNode* b)
    {
        swap(a->bookID, b->bookID);
        swap(a->title, b->title);
    }

    int NormalizeIndex(int index)
    {
        return ((index % count) + count) % count;
    }

public:
    CircularLinkedList()
    {
        head = nullptr;
        count = 0;
        cursor = 0;
    }

    int Size() { return count; }
    bool IsEmpty() { return count == 0; }
    bool IsNotEmpty() { return count > 0; }

    // Insert at node next to head (front)
    void Insert(BookNode* book)
    {
        if (book == nullptr)
            return;
        if (head == nullptr)
        {
            head = book;
            book->next = book;
        }
        else
        {
            book->next = head->next;
            head->next = book;
        }
        count++;
    }

    // Remove node next to head (front)
    BookNode* Remove()
    {
        if (IsEmpty())
            return nullptr;
        BookNode* first = head->next;
        if (first == head)
        {
            head = nullptr;
        }
        else
        {
            head->next = first->next;
        }
        first->next = nullptr;
        count--;
        return first;
    }

    // Insert at an index
    void InsertAt(BookNode* book, int index)
    {
        if (book == nullptr)
            return;
        if (count == 0)
        {
            head = book;
            book->next = book;
            count++;
            return;
        }
        if (index < 0)
            index = NormalizeIndex(index);
        if (index > count)
            index %= count;
        if (index == 0)
        {
            Insert(book);
            return;
        }
        if (index == count)
        {
            book->next = head->next;
            head->next = book;
            head = book;
            count++;
            return;
        }
        BookNode* previous = GetNodeAt(index - 1);
        book->next = previous->next;
        previous->next = book;
        count++;
    }

    // Remove at an index
    BookNode* RemoveAt(int index)
    {
        if (IsEmpty())
            return nullptr;
        index = NormalizeIndex(index);
        if (index == 0)
            return Remove();
        BookNode* previous = GetNodeAt(index - 1);
        BookNode* target = previous->next;
        previous->next = target->next;
        if (target == head)
            head = previous;
        target->next = nullptr;
        count--;
        return target;
    }

    // Swap data at two indexes
    void Swap(int index1, int index2)
    {
        if (IsEmpty())
            return;
        index1 = NormalizeIndex(index1);
        index2 = NormalizeIndex(index2);
        if (index1 == index2)
            return;
        BookNode* first = GetNodeAt(index1);
        BookNode* second = GetNodeAt(index2);
        SwapData(first, second);
    }

    // Sort by book ID (ascending)
    void Sort()
    {
        if (count < 2)
            return;
        BookNode* current = head->next;
        for (int i = 0; i < count - 1; i++)
        {
            BookNode* nextNode = current->next;
            for (int j = i + 1; j < count; j++)
            {
                if (current->bookID > nextNode->bookID)
                    SwapData(current, nextNode);
                nextNode = nextNode->next;
            }
            current = current->next;
        }
    }

    // Reverse the circular linked list
    void Reverse()
    {
        if (count < 2)
            return;
        BookNode* oldTail = head;
        BookNode* oldFirst = head->next;
        BookNode* previous = oldTail;
        BookNode* current = oldFirst;
        while (current != oldTail)
        {
            BookNode* nextNode = current->next;
            current->next = previous;
            previous = current;
            current = nextNode;
        }
        oldTail->next = previous;
        head = oldFirst;
    }

    // Print exactly count nodes
    void Print()
    {
        if (IsEmpty())
        {
            cout << "No books in the collection.\n";
            return;
        }
        BookNode* current = head->next;
        cout << "\nIndex\tBook ID\tTitle\n";
        cout << "-----------------------------------\n";
        for (int i = 0; i < count; i++)
        {
            cout << i << "\t"
                 << current->bookID << "\t"
                 << current->title << endl;
            current = current->next;
        }
        cout << "Total books: " << count << endl;
    }

    // Show the next book on the display screen and move forward.
    // After the last book it wraps back to the first (never reaches nullptr).
    void MoveToNext()
    {
        if (IsEmpty())
        {
            cout << "No books to display.\n";
            return;
        }
        int index = cursor % count;
        BookNode* book = GetNodeAt(index);
        cout << "Now showing [index " << index << "]: "
             << book->bookID << " - " << book->title;
        if (index == count - 1)
            cout << "   (last book, next wraps to first)";
        cout << endl;
        cursor = (index + 1) % count;
    }

    // Walk the whole circle and confirm the last node links back to the first
    void VerifyCircular()
    {
        if (IsEmpty())
        {
            cout << "List is empty.\n";
            return;
        }
        BookNode* first = head->next;
        BookNode* current = first;
        for (int i = 0; i < count; i++)
            current = current->next;
        cout << "Last node (" << head->title << ") -> next is first node ("
             << head->next->title << "): "
             << (head->next == first ? "YES" : "NO") << endl;
        cout << "Walking " << count << " steps from the first node returns to it: "
             << (current == first ? "YES" : "NO") << endl;
    }

    // Copy constructor
    CircularLinkedList(const CircularLinkedList& other)
    {
        head = nullptr;
        count = 0;
        cursor = 0;
        if (other.head == nullptr)
            return;
        BookNode* current = other.head->next;
        for (int i = 0; i < other.count; i++)
        {
            BookNode* copy = new BookNode{ current->bookID, current->title, nullptr };
            InsertAt(copy, count);
            current = current->next;
        }
    }

    // Assignment operator
    CircularLinkedList& operator=(const CircularLinkedList& other)
    {
        if (this == &other)
            return *this;
        while (IsNotEmpty())
            delete Remove();
        cursor = 0;
        if (other.head == nullptr)
            return *this;
        BookNode* current = other.head->next;
        for (int i = 0; i < other.count; i++)
        {
            BookNode* copy = new BookNode{ current->bookID, current->title, nullptr };
            InsertAt(copy, count);
            current = current->next;
        }
        return *this;
    }

    // Destructor
    ~CircularLinkedList()
    {
        while (IsNotEmpty())
            delete Remove();
    }
};

BookNode* CreateBook(int id, string title)
{
    BookNode* book = new BookNode;
    book->bookID = id;
    book->title = title;
    book->next = nullptr;
    return book;
}

// ---------- Input helpers ----------
int readInt(const string& prompt, int lo = numeric_limits<int>::min(),
            int hi = numeric_limits<int>::max())
{
    int value;
    while (true)
    {
        cout << prompt;
        if (cin >> value && value >= lo && value <= hi)
        {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
        cout << "Invalid input. Try again.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

string readLine(const string& prompt)
{
    string s;
    cout << prompt;
    getline(cin, s);
    return s;
}

void showMenu()
{
    cout << "\n========================================\n"
         << "   CIRCULAR LIBRARY BOOK DISPLAY SYSTEM\n"
         << "========================================\n"
         << "1. Display All Books\n"
         << "2. Insert Book at Head\n"
         << "3. Remove Book from Head\n"
         << "4. Insert Book at Index\n"
         << "5. Remove Book at Index\n"
         << "6. Swap Books\n"
         << "7. Sort Books by ID\n"
         << "8. Reverse Book Collection\n"
         << "9. Move to Next Book\n"
         << "10. Check Collection Status\n"
         << "11. Exit\n";
}

int main()
{
    CircularLinkedList library;
    int choice;

    do
    {
        showMenu();
        choice = readInt("Enter your choice: ", 1, 11);

        switch (choice)
        {
        case 1:
            library.Print();
            break;

        case 2:
        {
            int id = readInt("Book ID: ");
            string title = readLine("Book Title: ");
            library.Insert(CreateBook(id, title));
            cout << "Book inserted at head (next to pivot).\n";
            break;
        }

        case 3:
        {
            BookNode* removed = library.Remove();
            if (removed != nullptr)
            {
                cout << "Removed from head: " << removed->bookID
                     << " - " << removed->title << endl;
                delete removed;
            }
            else
                cout << "Collection is empty. Nothing to remove.\n";
            break;
        }

        case 4:
        {
            int id = readInt("Book ID: ");
            string title = readLine("Book Title: ");
            int index = readInt("Index (negative / large values wrap around): ");
            library.InsertAt(CreateBook(id, title), index);
            cout << "Book inserted.\n";
            break;
        }

        case 5:
        {
            if (library.IsEmpty())
            {
                cout << "Collection is empty. Nothing to remove.\n";
                break;
            }
            int index = readInt("Index to remove (wraps around circle): ");
            BookNode* removed = library.RemoveAt(index);
            cout << "Removed: " << removed->bookID << " - " << removed->title << endl;
            delete removed;
            break;
        }

        case 6:
        {
            if (library.Size() < 2)
            {
                cout << "Need at least two books to swap.\n";
                break;
            }
            int i1 = readInt("First index: ");
            int i2 = readInt("Second index: ");
            library.Swap(i1, i2);
            cout << "Books swapped.\n";
            break;
        }

        case 7:
            library.Sort();
            cout << "Collection sorted by Book ID.\n";
            break;

        case 8:
            library.Reverse();
            cout << "Collection reversed.\n";
            break;

        case 9:
            library.MoveToNext();
            break;

        case 10:
            if (library.IsNotEmpty())
            {
                cout << "\nCollection is NOT empty. Total books: " << library.Size() << endl;
                library.VerifyCircular();
            }
            else
                cout << "\nCollection is EMPTY.\n";
            break;

        case 11:
            cout << "\nExiting... releasing memory.\n";
            break;
        }
    } while (choice != 11);

    return 0;   // ~CircularLinkedList() deletes all remaining nodes
}
