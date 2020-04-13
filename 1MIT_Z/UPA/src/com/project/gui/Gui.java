package com.project.gui;




import javax.imageio.ImageIO;
import javax.swing.*;
import javax.swing.filechooser.FileSystemView;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;

public class Gui {
   public static JPanel panel= new BackgroundForGui();
   public static JPanel indexPanel = new BackgroundForGui();
   public static JPanel searchPanel = new BackgroundForGui();
   public static JPanel loginPanel = new BackgroundForGui();
   public static String  pathToSelectedImage;
   public static String  pathToDirectorysearch;
   public static  JFrame frame = new JFrame("Gui");
   public static String  indexSelectedmethods;
   public static String  searchSelectedmethods;
   public static  Dimension screenSize = Toolkit.getDefaultToolkit().getScreenSize();
   public static  int sizewidth=(int)(0.625*screenSize.width);
   public static int sizeheigt=(int)(0.7275*screenSize.height);
   //(int)(0.7275*screenSize.height);
   public static  int SizeWidth3= sizewidth/3;
   public static int SizeWidth50= sizewidth/50;
   public static int sizeHeigt06= (int) (0.6*sizeheigt);
   public static int sizeWidth2= (int) (0.5*sizewidth);



   // public static JPanel indexPanel = new JPanel();
    public static void main(String[] args) throws IOException {
      //  Dimension screenSize = Toolkit.getDefaultToolkit().getScreenSize();
       // int sizewidth=(int)(0.625*screenSize.width);
        //int sizeheigt=(int)(0.7275*screenSize.height);
        //int SizeWidth3= sizewidth/3;
        //int SizeWidth50= sizewidth/50;
       // int sizeHeigt06= (int) (0.6*sizeheigt);






        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setSize(sizewidth,sizeheigt);
        //frame.pack();
        frame.setResizable(false);
        //frame.setVisible(true);
        //frame.setLayout(null);
       ////////nastaveni main panel/////////////////////////////////
        ////////////////////////////////////////////////////////////
        panel.setLayout(null);
        Icon icon = new ImageIcon(Gui.class.getClassLoader().getResource("img/search.png"),"Search");

        JButton buttonSearch = new JButton(icon);
        Icon icon1 = new ImageIcon(Gui.class.getClassLoader().getResource("img/index.png"),"Index");
        JButton buttonIndex = new JButton(icon1);
        buttonSearch.setBounds(0,100,100,100);
        buttonIndex.setBounds(0,0,100,100);
        buttonIndex.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                frame.remove(panel);
                frame.add(indexPanel);
                frame.validate();
                frame.repaint();
            }
        });
        buttonSearch.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                frame.remove(panel);
                frame.add( searchPanel);
                frame.validate();
                frame.repaint();
            }
        });

        panel.add(buttonSearch);
        panel.add(buttonIndex);
        ////////////////nastaveni login panel//////////////
        try {
            login();
        } catch (IOException e) {
            e.printStackTrace();
        }
        ///////////////////////////////////////////////////////////


        //////////nastaveni search panel////////////////
        try {
            setSearchPanel();
        } catch (IOException e) {
            e.printStackTrace();
        }
        ///////////////////konec nastaveni//////////////////////////

        ////// nastaveni index panel////////////////////
        ////////////////////////////////////////////////
        indexPanel.setLayout(null);
        JButton open = new JButton("Open");
        open.setBounds(0,100,200,50);
        JButton rotate = new JButton("Rotate");
        rotate.setBounds(0,200,200,50);
        /////////////////////////////////////////////////////////
        JLabel label = new JLabel("Multimedia Database");//initialize the label
        label.setBounds(350,0,400,100);
        label.setForeground(Color.WHITE);
        label.setFont(new Font("HelveticaNeue-Italic", Font.BOLD, 30));
       ///////////////////////////////////label 1best
        JLabel labelB = new JLabel("Best Result");//initialize the label
        labelB.setBounds(0,sizeHeigt06,200,50);
        labelB.setForeground(Color.WHITE);
        labelB.setFont(new Font("HelveticaNeue-Italic", Font.BOLD, 22));
       ///////////////////////////secondbest
        JLabel labelB1 = new JLabel("2.Best Result");//initialize the label
        labelB1.setBounds(SizeWidth3,sizeHeigt06,200,50);
        labelB1.setForeground(Color.WHITE);
        labelB1.setFont(new Font("HelveticaNeue-Italic", Font.BOLD, 22));
        /////////////////////////////////////3 best///////////
        JLabel labelB2 = new JLabel("3.Best result");//initialize the label
        labelB2.setBounds(2*(SizeWidth3),sizeHeigt06,200,50);
        labelB2.setForeground(Color.WHITE);
        labelB2.setFont(new Font("HelveticaNeue-Italic", Font.BOLD, 22));
        ////////////////////////////////////////selected image//////////////////////////
        JLabel labelB3 = new JLabel("Selected Image");//initialize the label
        labelB3.setBounds(SizeWidth3,(int) (sizeheigt*0.22),200,50);
        labelB3.setForeground(Color.WHITE);
        labelB3.setFont(new Font("HelveticaNeue-Italic", Font.BOLD, 22));
        try {
            BufferedImage image = ImageIO.read(BackgroundForGui.class.getClassLoader().getResource("img/backGround.png"));
            JLabel labelObr=new JLabel(new ImageIcon(image.getScaledInstance(SizeWidth3-20, sizeheigt-sizeHeigt06-76, Image.SCALE_FAST)));
            labelObr.setBounds(0,sizeHeigt06+40,SizeWidth3-20,sizeheigt-sizeHeigt06-76);
            indexPanel.add(labelObr);
            BufferedImage image1 = ImageIO.read(BackgroundForGui.class.getClassLoader().getResource("img/backGround1.jpg"));
            JLabel labelObr1=new JLabel(new ImageIcon(image1.getScaledInstance(SizeWidth3-20, sizeheigt-sizeHeigt06-76, Image.SCALE_FAST)));
            labelObr1.setBounds(SizeWidth3,sizeHeigt06+40,SizeWidth3-20,sizeheigt-sizeHeigt06-76);
            indexPanel.add(labelObr1);
            BufferedImage image2 = ImageIO.read(BackgroundForGui.class.getClassLoader().getResource("img/backGround12.jpg"));
            JLabel labelObr2=new JLabel(new ImageIcon(image2.getScaledInstance(SizeWidth3-20, sizeheigt-sizeHeigt06-76, Image.SCALE_FAST)));
            labelObr2.setBounds(2*(SizeWidth3),sizeHeigt06+40,SizeWidth3-20,sizeheigt-sizeHeigt06-76);
            indexPanel.add(labelObr2);

        }
        catch(Exception e) {

        }
        open.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                JFileChooser jfc = new JFileChooser(FileSystemView.getFileSystemView().getHomeDirectory());
                jfc.setFileSelectionMode(JFileChooser.FILES_ONLY);
                int returnValue = jfc.showOpenDialog(null);
                if (returnValue == JFileChooser.APPROVE_OPTION) {
                    File selectedFile = jfc.getSelectedFile();
                    pathToSelectedImage=selectedFile.getAbsolutePath();
                    try {
                        BufferedImage image3 = ImageIO.read(new File(pathToSelectedImage));
                        JLabel labelObr3=new JLabel(new ImageIcon(image3.getScaledInstance(SizeWidth3-20, sizeheigt-sizeHeigt06-76, Image.SCALE_FAST)));
                        labelObr3.setBounds(SizeWidth3, (int) (sizeheigt*0.3),SizeWidth3-20,sizeheigt-sizeHeigt06-76);
                        indexPanel.add(labelObr3);
                        frame.validate();
                        frame.repaint();
                    } catch (IOException ex) {
                        ex.printStackTrace();
                    }

                    //System.out.println(pathToDirectoryindex);
                }
            }
        });
        JButton startIndex = new JButton("Search Image");
        startIndex.setBounds(0,151,200,50);
        startIndex.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
               // try {
                //  // zde se zavola to co to ma delat
                 //     Indexer.index(pathToDirectoryindex,indexSelectedmethods);
               // } catch (IOException e1) {
               //     e1.printStackTrace();
             //   }
            }
        });
        Icon icon2 = new ImageIcon(Gui.class.getClassLoader().getResource("img/back.png"),"Back");
        JButton buttonback = new JButton(icon2);
        buttonback.setBounds(0,0,45,30);
        buttonback.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                frame.remove(indexPanel);
                frame.add(panel);
                frame.validate();
                frame.repaint();
            }
        });
        String[] methodTitles = new String[] {"CEDD","Gabor",
                "FCTH","Tamura","AutoColorCorrelogram"};
        JComboBox<String> methodList = new JComboBox<>(methodTitles);
        //add(bookList);
        methodList.setBounds(150,100,100,30);
        indexSelectedmethods = (String) methodList.getSelectedItem();

        // System.out.println("You seleted the book: " + selectedMethods);


        indexPanel.add(labelB);
        indexPanel.add(labelB1);
        indexPanel.add(labelB2);
        indexPanel.add(labelB3);
        indexPanel.add(buttonback);
        indexPanel.add(label);
        indexPanel.add(open);
        indexPanel.add(rotate);
        indexPanel.add(startIndex);
        indexPanel.add(methodList);
        ///////////////konec nastaveni index panel////////////

        frame.add(loginPanel);
       // frame.add(loginPanel);
        frame.setVisible(true);

    }

    public static void setSearchPanel() throws FileNotFoundException, IOException {
        searchPanel.setLayout(null);
        JLabel image = new JLabel();
        JLabel label = new JLabel("Search with selected image");//initialize the label
        label.setBounds(250,0,400,100);
        label.setFont(new Font("Serif", Font.BOLD, 30));
        JButton openSearch = new JButton("Open");
        openSearch.setBounds(0,100,200,50);
        JButton rotate = new JButton("Rotate");
        rotate.setBounds(300,100,200,50);
        openSearch.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                JFileChooser jfc = new JFileChooser(FileSystemView.getFileSystemView().getHomeDirectory());
                jfc.setFileSelectionMode(JFileChooser.DIRECTORIES_ONLY);
                int returnValue = jfc.showOpenDialog(null);
                if (returnValue == JFileChooser.APPROVE_OPTION) {
                    File selectedFile = jfc.getSelectedFile();
                    pathToDirectorysearch=selectedFile.getAbsolutePath();
                    System.out.println(pathToDirectorysearch);

                    //Indexer.openImageAndSave(pathToFile,"SearchImage",true);
                    //ImageIcon icon2 = new ImageIcon("/Users/pavelhosek/Desktop/dicomy/SearchImage.png","Zobrazeni obrazku");
                 //   Image img =icon2.getImage().getScaledInstance(400, 400,Image.SCALE_DEFAULT);
                    //ImageIcon icon5 =new ImageIcon(img);
                   // image.setIcon(icon5);
                  //  image.setBounds(200,100,400,400);
                }
            }
        });
        JButton StartSearch = new JButton("Start Searching");
        StartSearch.setBounds(0,151,120,50);
        Icon icon3 = new ImageIcon(Gui.class.getClassLoader().getResource("img/back.png"),"Back");
        JButton buttonback1 = new JButton(icon3);
        buttonback1.setBounds(0,0,45,30);
        buttonback1.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                frame.remove(searchPanel);
                frame.add(panel);
                frame.validate();
                frame.repaint();
            }
        });
        StartSearch.addActionListener(new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
              //      Searcher.search(pathToDirectorysearch,50,10,Searcher.returnRightclass(searchSelectedmethods));
            }
        });
        String[] methodTitles = new String[] {"CEDD","Gabor",
                "FCTH","Tamura","AutoColorCorrelogram"};
        JComboBox<String> methodList = new JComboBox<>(methodTitles);
        //add(bookList);
        methodList.setBounds(150,100,100,30);
        searchSelectedmethods = (String) methodList.getSelectedItem();
        // System.out.println("You seleted the book: " + selectedMethods);


        searchPanel.add(label);
        searchPanel.add(buttonback1);
        searchPanel.add(openSearch);
        searchPanel.add(StartSearch);
        searchPanel.add(image);
        searchPanel.add(methodList);
    }
    public static void login() throws FileNotFoundException, IOException {
         final long serialVersionUID = 1;
         JTextField textField;
         JPasswordField passwordField;
         JButton btnNewButton;
         JLabel label;
        loginPanel.setLayout(null);


        JLabel lblNewLabel = new JLabel("Login");
        lblNewLabel.setForeground(Color.WHITE);
        lblNewLabel.setFont(new Font("Times New Roman", Font.PLAIN, 46));
        lblNewLabel.setBounds(sizeWidth2-80, 13, 273, 93);
        loginPanel.add(lblNewLabel);

        textField = new JTextField();
        textField.setFont(new Font("Tahoma", Font.PLAIN, 32));
        textField.setBounds(sizeWidth2, (int) (sizeheigt*0.2), 281, 68);
        loginPanel.add(textField);
        textField.setColumns(10);

        passwordField = new JPasswordField();
        passwordField.setFont(new Font("Tahoma", Font.PLAIN, 32));
        passwordField.setBounds(sizeWidth2,  (int) (sizeheigt*0.35), 281, 68);
        loginPanel.add(passwordField);

        JLabel lblUsername = new JLabel("Username");
        lblUsername.setBackground(Color.BLACK);
        lblUsername.setForeground(Color.WHITE);
        lblUsername.setFont(new Font("Tahoma", Font.PLAIN, 31));
        lblUsername.setBounds(sizeWidth2-250, (int) (sizeheigt*0.2), 193, 52);
        loginPanel.add(lblUsername);

        JLabel lblPassword = new JLabel("Password");
        lblPassword.setForeground(Color.WHITE);
        lblPassword.setBackground(Color.CYAN);
        lblPassword.setFont(new Font("Tahoma", Font.PLAIN, 31));
        lblPassword.setBounds(sizeWidth2-250,  (int) (sizeheigt*0.35), 193, 52);
        loginPanel.add(lblPassword);

        btnNewButton = new JButton("Login");
        btnNewButton.setFont(new Font("Tahoma", Font.PLAIN, 26));
        btnNewButton.setBounds(sizeWidth2, (int) (sizeheigt*0.50), 162, 73);
        loginPanel.add(btnNewButton);
        btnNewButton.addActionListener(new ActionListener() {

            public void actionPerformed(ActionEvent e) {
                String userName = textField.getText();
                String password = passwordField.getText();
                try {
                    ///zde pude pripojeni
                    if  (userName.contentEquals("Pavel")){
                        frame.remove(loginPanel);
                        frame.add(panel);
                        frame.validate();
                        frame.repaint();
                    }
                    else {
                        JOptionPane.showMessageDialog(btnNewButton, "Wrong Username & Password");
                    }


                } catch (Exception ea) {

                }
            }
        });




    }


}
