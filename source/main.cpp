#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <limits>

std::vector <int> getReferenceList(const char* filename, int& nFrames)
{
    std::vector <int> referenceList;

    std::ifstream file;
    file.open(filename);

    file >> nFrames;        //gets number of frames in memory

    while(file.good())
    {
        int newReference;

        file >> newReference;       

        referenceList.push_back(newReference);        
    }

    file.close();

    return referenceList;
}

int FIFO(std::vector <int> referenceList, const int nFrames)
{
    std::vector <int> frameArray(nFrames, -1);

    int nErrors = 0;

    int nextEliminated = 0;

    for(unsigned int referenceId = 0; referenceId < referenceList.size(); ++referenceId)    //runs through references
    { 
        int reference = referenceList[referenceId];     //gets current reference to be tested/substituted
        bool gotIn = false;        

        for(int i = 0; i < nFrames; ++i)                //runs through frames and...
        {
            if(frameArray[i] == reference)              //checks if the reference is already in memory
            {
                //HIT!!
                gotIn = true; 
                break;
            }
            else if(frameArray[i] == -1)                //checks if there's one free to use
            {   
                //found free frame to get into
                frameArray[i] = reference;
                gotIn = true;
                ++nErrors;
                break;
            }            
        }

        if(!gotIn)                                      //if the reference wasnt found or got in, it has to eliminate one of the current ones
        {
            ++nErrors;

            frameArray[nextEliminated] = reference;

            nextEliminated = (nextEliminated + 1) % nFrames;    //the references are substituted in a circular fashion
        }
        /*
        std::cout << reference << " --> ";
        for(int i = 0; i < nFrames; ++i)
        {
            std::cout << frameArray[i] << " ";
        }
        std::cout << std::endl;
        */
    }   

    return nErrors;
}

struct referenceCase{
    int nextCaseDistance = 99999;
    int nextCaseId = 0;
};

static bool myCompare(struct referenceCase ref1, struct referenceCase ref2)
{
    return (ref1.nextCaseDistance < ref2.nextCaseDistance);
}

int OTM(std::vector <int> referenceList, const int nFrames)
{
    std::vector <int> frameArray(nFrames, -1);
    
    int nErrors = 0;

    //struct referenceCase fodderReference{9999, 0};

    for(unsigned int referenceId = 0; referenceId < referenceList.size(); ++referenceId)    //runs through references
    {
        int reference = referenceList[referenceId];     //gets current reference to be tested/substituted
        bool gotIn = false;        

        for(int i = 0; i < nFrames; ++i)                //runs through frames and...
        {
            if(frameArray[i] == reference)              //checks if the reference is already in memory
            {
                //HIT!!
                gotIn = true; 
                break;
            }
            else if(frameArray[i] == -1)                //checks if there's one free to use
            {   
                //found free frame to get into
                frameArray[i] = reference;
                gotIn = true;
                ++nErrors;
                break;
            }            
        }

        if(!gotIn)
        {   
            ++nErrors;
            
            int nextEliminated;
            std::vector <struct referenceCase> nextCase(nFrames);

            for(int j = 0; j < nFrames; ++j)                                 //for each reference in memory
            {
                for(unsigned int i = referenceId; i < referenceList.size(); ++i)      //finds its 
                {
                    if(referenceList[i] == frameArray[j])
                    {
                        nextCase[j].nextCaseDistance = i - referenceId;
                        nextCase[j].nextCaseId = j;
                        break;
                    }
                }
            } 

            std::sort(nextCase.begin(), nextCase.end(), myCompare);         //sort next cases by distance
            
            /*
            std::cout << "[ ";
            for(int i = 0; i < nFrames; ++i)
            {
                std::cout << "("<< nextCase[i].nextCaseDistance << " " << nextCase[i].nextCaseId << ") ";
            }
            std::cout << "]" << std::endl;
            */

            nextEliminated = nextCase[nFrames - 1].nextCaseId;                  //the substituted frame is the one with the most distant next case
        
            frameArray[nextEliminated] = reference;
        }

        /*
        std::cout << reference << " --> ";
        for(int i = 0; i < nFrames; ++i)
        {
            std::cout << frameArray[i] << " ";
        }
        std::cout << std::endl;
        */
    }

    return nErrors;
}

int LRU(std::vector <int> referenceList, const int nFrames)
{
    std::vector <int> frameArray(nFrames, -1);
    std::vector <int> refTimeArray(nFrames, -1);
    
    int nErrors = 0;    

    for(unsigned int referenceId = 0; referenceId < referenceList.size(); ++referenceId)    //runs through references. referenceId works as the program counter
    {
        int reference = referenceList[referenceId];     //gets current reference to be tested/substituted
        bool gotIn = false;        

        for(int i = 0; i < nFrames; ++i)                //runs through frames and...
        {
            if(frameArray[i] == reference)              //checks if the reference is already in memory
            {
                //HIT!!
                gotIn = true;
                refTimeArray[i] = referenceId; 
                break;
            }
            else if(frameArray[i] == -1)                //checks if there's one free to use
            {   
                //found free frame to get into
                frameArray[i] = reference;
                refTimeArray[i] = referenceId;
                gotIn = true;
                ++nErrors;
                break;
            }            
        }

        if(!gotIn)
        {   
            ++nErrors;
            
            int nextEliminated = 0;                                             //index of element to be eliminated
            int nextEliminatedValue = 99999;            

            for(int j = 0; j < nFrames; ++j)                                    //for each reference in memory
            {
                if(refTimeArray[j] < nextEliminatedValue)                       //finds lowest counter
                {
                    nextEliminatedValue = refTimeArray[j];
                    nextEliminated = j;                                         //updates next eliminated array based on finds
                }
            } 
            
            frameArray[nextEliminated] = reference;
            refTimeArray[nextEliminated] = referenceId;
        } 

        /*
        std::cout << "[ ";
        for(int i = 0; i < nFrames; ++i)
        {
            std::cout << refTimeArray[i] << " ";
        }
        std::cout << "]" << std::endl;

        std::cout << reference << " --> ";
        for(int i = 0; i < nFrames; ++i)
        {
            std::cout << frameArray[i] << " ";
        }
        std::cout << std::endl << std::endl;
        */
    }

    return nErrors;
}

int main()
{
    int nFrames;
    std::vector <int> referenceList = getReferenceList("memoria.txt", nFrames);     

    std::cout << "FIFO " << FIFO(referenceList, nFrames) << std::endl;
    std::cout << "OTM " << OTM(referenceList, nFrames) << std::endl;
    std::cout << "LRU " << LRU(referenceList, nFrames) << std::endl;    

    return 0;
}