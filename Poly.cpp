#include "Poly.h"
#include <math.h>
#include <iostream>
using namespace std;
Poly::Poly()
{
	head = new PolyNode(-1, 0.0, NULL);
}

Poly::Poly(const std::vector<int>& deg, const std::vector<double>& coeff)
{
	head = new PolyNode(-1, 0.0, NULL);
	PolyNode* temp = head;
	for (int i = 0; i < deg.size(); i++){
		temp -> next = new PolyNode(deg[i], coeff[i], NULL);
		temp = temp->next;
	}
}

Poly::~Poly()
{
	PolyNode* temp = head;
	PolyNode* del;
	while(head->next != NULL){
		del = head->next;
		head->next = head->next->next;
		delete del;	
	}
	

}

void Poly::addMono(int i, double c)
{
	//time comp  O(n)
	PolyNode *temp = head;
	while (true){
		if(temp->next == NULL){
			temp->next = new PolyNode(i, c, NULL);
			break;
		}else if(temp->next->deg < i){
			PolyNode* add = new PolyNode(i, c, temp->next);
			temp->next = add;
			break;
		}else if(temp->next->deg == i){
			temp->next->coeff += c;
			break;
		}else{ //next element is higher degree
			temp = temp->next;
		}

	}
	
	temp = head;
	PolyNode* del;
	while(temp->next != NULL){
		if (temp->next->coeff == 0.0){
			del = temp->next;
			temp->next = temp->next->next;
			delete del;
			break;
		}else{
			temp = temp-> next;
		}
		
	}
}

void Poly::addPoly(const Poly& p)
{
	//time comp O(n^2)
	PolyNode* cursor = p.head;
	while(cursor->next != NULL){
		addMono(cursor->next->deg, cursor->next->coeff);
		cursor = cursor->next;
	}
}

void Poly::multiplyMono(int i, double c)
{
	//time comp O(n)		
	PolyNode* temp = head;
	if(c == 0.0){
		delete this;
	}else{

		while(temp->next != NULL){
			temp->next->deg += i;
			temp->next->coeff *=c;
			temp = temp->next;
		}
	}
}

void Poly::multiplyPoly(const Poly& p)
{
	int i = p.getTermsNo();
	Poly sum = *(new Poly());
	PolyNode* pe = p.head; 
	Poly copy;
	while(pe->next != NULL){
		pe=pe->next;
		copy = *(new Poly()); 
		this->duplicate(copy);
		copy.multiplyMono(pe->deg, pe->coeff);
		sum.addPoly(copy);
	}
	//cannot reassign this to product (anachronism) so we will do it roundabout
	//in essence, subtract everything, then add the sum (mult by 0 deletes object)
	Poly neg = *(new Poly);
	this->duplicate(neg);
	neg.multiplyMono(0,-1);
	this->addPoly(neg);
	this->addPoly(sum);


}

void Poly::duplicate(Poly& outputPoly)
{
	//time complexity O(n)
	PolyNode* temp = outputPoly.head;
	PolyNode* traverse = head;
	int x = outputPoly.getTermsNo();
	while(traverse->next != NULL){
		temp -> next = new PolyNode(traverse->next->deg, traverse->next->coeff, NULL);
		temp = temp->next;
		traverse = traverse->next;
	}
}
int Poly::getDegree()
{
	//time comp O(1)
	if (head->next != NULL){
		return head->next->deg;
	}else{
		return -1;
	}

}

PolyNode* Poly::getHeadNode(){
	return head;
}
int Poly::getTermsNo() const
{
	//time comp O(n)
	int i = 0;
	PolyNode* temp = head;
	while(temp->next != NULL){
		i++;
		temp = temp->next;
	}
	// TODO
	return i;//change this after completing this function
}

double Poly::evaluate(double x)
{
	//time comp O(n)
	double sum = 0.0;
	PolyNode* temp = head;
	while(temp-> next != NULL){
		temp = temp->next;
		sum += pow(x, temp->deg) * temp->coeff;
	}
	return sum;//change this after completing this function
}

string Poly::toString()
{
	//time complexity O(n)
	//not using C++ 20 so we rely on janky formatting
	string o = "";
	o += "degree=" + to_string(this->getDegree())+"; ";
	PolyNode* temp = head;
	while(temp-> next != NULL){
		temp = temp->next;
		o+= "a(" + to_string(temp->deg) + ")=" + to_string(temp->coeff) + "; "; 
	}
	o+="\n";
	return o;
}