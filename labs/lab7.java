
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Random;
import java.util.Scanner;

public class Array {
    private final int[] arr;
    private final int size;

    public Array(int size) {
        this.size = size;
        this.arr = new int[size];
    }

    public void fillWithRandom() {
        Random random = new Random();
        for (int i = 0; i < size; i++) {
            arr[i] = random.nextInt(100);
        }
    }

    public void fillFromKeyboard() {
        Scanner scanner = new Scanner(System.in);
        System.out.println("Введите " + size + " чисел:");
        for (int i = 0; i < size; i++) {
            arr[i] = scanner.nextInt();
        }
    }

    public void fillFromFile(String filename) throws IOException {
        Scanner fileScanner = new Scanner(new File(filename));
        for (int i = 0; i < size && fileScanner.hasNextInt(); i++) {
            arr[i] = fileScanner.nextInt();
        }
        fileScanner.close();
    }

    public void print() {
        System.out.print("Массив: ");
        for (int i = 0; i < size; i++) {
            System.out.print(arr[i] + " ");
        }
        System.out.print('\n');
    }

    public void printAndCountInRange(int lowerBound, int upperBound, String outputFilename) throws IOException {
        int count = 0;
        FileWriter writer = new FileWriter(outputFilename);
        System.out.println("Числа в диапазоне [" + lowerBound + ", " + upperBound + "]:");

        for (int i = 0; i < size; i++) {
            if (arr[i] >= lowerBound && arr[i] <= upperBound) {
                System.out.print(arr[i] + " ");
                writer.write(arr[i] + " ");
                count++;
            }
        }

        System.out.print('\n');
        writer.write('\n');

        System.out.println("Количество чисел в диапазоне: " + count);
        writer.write("Количество чисел в диапазоне: " + count + "\n");
        writer.close();
    }

    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);

        System.out.print("Введите размер массива: ");
        int size = scanner.nextInt();

        Array array = new Array(size);

        System.out.println("Выберите метод заполнения массива:");
        System.out.println("1. Случайные числа");
        System.out.println("2. Ввод с клавиатуры");
        System.out.println("3. Чтение из файла");
        int choice = scanner.nextInt();

        try {
            switch (choice) {
                case 1:
                    array.fillWithRandom();
                    break;
                case 2:
                    array.fillFromKeyboard();
                    break;
                case 3:
                    System.out.print("Введите имя файла: ");
                    String filename = scanner.next();
                    array.fillFromFile(filename);
                    break;
                default:
                    System.out.println("Неверный выбор.");
                    return;
            }

            array.print();

            System.out.print("Введите нижнюю границу диапазона: ");
            int lowerBound = scanner.nextInt();
            System.out.print("Введите верхнюю границу диапазона: ");
            int upperBound = scanner.nextInt();

            array.printAndCountInRange(lowerBound, upperBound, "output.txt");
        } catch (IOException e) {
            System.out.println("Ошибка при работе с файлом: " + e.getMessage());
        }

        scanner.close();
    }
}
