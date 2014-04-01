class node
{
   private:
     int child;
     int sibling;
	
   public:
     node();
     void setChild (int c);
     void setSibling (int s);
     int getChild ();
     int getSibling ();
};

