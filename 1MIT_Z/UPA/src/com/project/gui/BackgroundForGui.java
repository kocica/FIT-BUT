package com.project.gui;
import javax.imageio.ImageIO;
import javax.swing.*;
import java.awt.*;
import java.awt.image.BufferedImage;
import java.io.IOException;

    public class BackgroundForGui extends JPanel {
        protected void paintComponent(Graphics g) {
            super.paintComponent(g);

            final BufferedImage image;

            try {
                Dimension screenSize = Toolkit.getDefaultToolkit().getScreenSize();
               // int sizewidth=(int)(0.625*screenSize.width);
                //int sizeheigt=(int)(0.7275*screenSize.height);
                int sizewidth=(int)(screenSize.width);
                int sizeheigt=(int)(screenSize.height);
                image = ImageIO.read(BackgroundForGui.class.getClassLoader().getResource("img/backGround.png"));
                g.drawImage(image.getScaledInstance(sizewidth, sizeheigt,image.SCALE_DEFAULT), 0, 0, null);

            } catch (IOException e) {
                e.printStackTrace();
            }


        }
    }



