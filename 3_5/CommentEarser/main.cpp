#include <iostream>



int main()
{
        using namespace std;       
        
		struct CommState {
			bool slashComment;	// reading under slash comment state
			bool starComment;   // reading under star comment state
			bool prevState0;
			bool prevState1;
			char ChPrev;
		} comm = { false, false, false, false, char_traits<char>::eof() };
		
		
        while (cin || comm.ChPrev != char_traits<char>::eof()) {
                char ch = cin.get();                
                
				switch (ch) {
					case '\n':
						comm.slashComment = false;
						break;;
					case '/':
						if (!comm.starComment && comm.ChPrev == '/')
							comm.slashComment = true;
						if (comm.ChPrev == '*')
							comm.starComment = false;						
						break;
					case '*':
						if (!comm.slashComment && comm.ChPrev == '/')
							comm.starComment = true;												
						break;											
				}								
				if (!comm.slashComment && !comm.starComment &&
				!comm.prevState0 && !comm.prevState1)
					cout.put(comm.ChPrev);
				
				comm.prevState1 = comm.prevState0;
				comm.prevState0 = comm.slashComment | comm.starComment;
				comm.ChPrev = ch;
        }
}